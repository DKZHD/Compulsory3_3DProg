#include "Terrain.h"
#include "../Mesh/Mesh.h"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "../Camera/camera.h"
#include <glm/ext/vector_float2.hpp>
#include <cmath>
#include <iostream>
#include "../Mesh/Player.h"
#include <glad/glad.h>
#include "../Shader/shader.h"
#include <glm/gtc/type_ptr.hpp>
void Terrain::CreateTerrain(float resolution, float size)
{
    numPrRow = 0;
    Size = size;
    bool countRows = true;
	float step = 1.f/resolution;
	int startIndex = 0;
	for(float y = 0.f; y<size;y+=step)
	{
		for(float x = 0.f; x<size;x+=step)
		{
			vertices.emplace_back(glm::vec3(x, (glm::sin(y/2)*glm::sin(x)), y));
            countRows ? numPrRow++ : 0;
		}
        countRows = false;
	}
	for (float i = 0.f; i < size-step;i += step)
    {
            for (float x = 0.f; x < size - step; x += step)
            {
                indices.push_back(startIndex);
                indices.push_back(startIndex + 1);
                indices.push_back(startIndex + numPrRow + 1);

                const glm::vec3 normal1 = glm::cross(vertices[startIndex + 1] - vertices[startIndex], vertices[startIndex + numPrRow + 1] - vertices[startIndex]);
                vertices[startIndex].Normal += normal1;
                vertices[startIndex + 1].Normal += normal1;
                vertices[startIndex + 11].Normal += normal1;

                indices.push_back(startIndex + numPrRow + 1);
                indices.push_back(startIndex + numPrRow);
                indices.push_back(startIndex);

                const glm::vec3 normal2 = glm::cross(vertices[startIndex + numPrRow] - vertices[startIndex + numPrRow+1], vertices[startIndex+1] - vertices[startIndex + numPrRow+1]);
                vertices[startIndex + numPrRow].Normal += normal2;
                vertices[startIndex + numPrRow + 1].Normal += normal2;
                vertices[startIndex].Normal += normal2;
                startIndex++;
            }
        startIndex++;
	}
    for (auto& vertex : vertices)
    {
        vertex.Normal = glm::normalize(vertex.Normal);
    }
    SetupBuffers();
}


void Terrain::Draw(unsigned int& shader_program)
{
    glBindVertexArray(VAO);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));
    glUniform3fv(glGetUniformLocation(shader_program, "material.diffuse"), 1, glm::value_ptr(glm::vec3(0.0, 0.7f, 0.f)));
    glUniform3fv(glGetUniformLocation(shader_program, "material.ambient"), 1, glm::value_ptr(glm::vec3(0.8f)));
    glUniform3fv(glGetUniformLocation(shader_program, "material.specular"), 1, glm::value_ptr(glm::vec3(0.0f)));
    glUniform1f(glGetUniformLocation(shader_program, "material.shininess"), 64.f);
    glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, nullptr);
}
std::pair<bool, float> Terrain::CheckHeight(glm::vec3 position)
{
    glm::vec2 PosVec2 = glm::vec2(position.x, position.z);
    float GridSize = Size / static_cast<float>(numPrRow);
    int terrainLocationX = static_cast<int>(glm::floor(PosVec2.x / GridSize));
    int terrainLocationZ = static_cast<int>(glm::floor(PosVec2.y / GridSize));
    if (terrainLocationX > vertices.size() - 1 || terrainLocationZ > vertices.size() - 1 || terrainLocationX < 0 || terrainLocationZ < 0)
        return std::make_pair(false, 0.f);

    float xPos = std::fmod(PosVec2.x, GridSize);
    float zPos = std::fmod(PosVec2.y, GridSize);

    int p1Index;
    int p2Index;
    int p3Index;

    if (xPos <= 1 - zPos) {
        p1Index = terrainLocationX + terrainLocationZ * numPrRow;
        p2Index = (terrainLocationX + 1) + terrainLocationZ * numPrRow;
        p3Index = terrainLocationX + (terrainLocationZ + 1) * numPrRow;
    }
    else {
        p1Index = (terrainLocationX + 1) + terrainLocationZ * numPrRow;
        p2Index = (terrainLocationX + 1) + (terrainLocationZ + 1) * numPrRow;
        p3Index = terrainLocationX + (terrainLocationZ + 1) * numPrRow;
    }
    if (p3Index >= vertices.size() - 1) {
        return std::make_pair(false, 0.f);
    }
    float height = BarycentricCalculation(vertices[p1Index].position, vertices[p2Index].position, vertices[p3Index].position, PosVec2);
    if (position.y < height) {
        return std::make_pair(true, height);
    }
    return std::make_pair(false, 0.f);
}

float Terrain::BarycentricCalculation(glm::vec3 p_1, glm::vec3 p_2, glm::vec3 p_3, glm::vec2 pos)
{
    float determinant = (p_2.z - p_3.z) * (p_1.x - p_3.x) + (p_3.x - p_2.x) * (p_1.z - p_3.z);
    float lambda1 = ((p_2.z - p_3.z) * (pos.x - p_3.x) + (p_3.x - p_2.x) * (pos.y - p_3.z)) / determinant;
    float lambda2 = ((p_3.z - p_1.z) * (pos.x - p_3.x) + (p_1.x - p_3.x) * (pos.y - p_3.z)) / determinant;
    float lambda3 = 1.f - lambda1 - lambda2;
    return lambda1*p_1.y + lambda2*p_2.y+lambda3*p_3.y;
}

void Terrain::SetupBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
}
