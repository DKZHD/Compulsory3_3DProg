#include "Mesh.h"
#include <glad/glad.h>
#include <cmath>
#include <glm/gtc/constants.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>

SphereObject::SphereObject(glm::vec3 position, int subdivisions)
{
	this->position = position;
	float radius = 0.5f;
	for (int i = 0; i < subdivisions/2; i++)
	{
		float phi = static_cast<float>(i) / (subdivisions - 1) * 2.f * glm::pi<float>();
		float y = std::cos(phi) * radius;
		float sphereRadius = std::sin(phi) * radius;

		for (int j = 0; j < subdivisions; j++) {
			float Theta = static_cast<float>(j) / (subdivisions - 1) * 2.f * glm::pi<float>();
			float x = sphereRadius * std::sin(Theta);
			float z = sphereRadius * std::cos(Theta);
				vertices.emplace_back(glm::vec3(x, -y, z));
				vertices.back().Normal = glm::vec3(0.5f);
				if (vertices.back().position.y < lowestpoint.second)
					lowestpoint = std::make_pair(vertices.size() - 1, vertices.back().position.y);
		}
		int sphereStartIndex = 0;
		for (int f = 0; f < subdivisions - 1; f++) {
			for (int g = 0; g < subdivisions - 1; g++) {

				int idx0 = sphereStartIndex + f * subdivisions + g;
				int idx1 = idx0 + 1;
				int idx2 = sphereStartIndex + ((f + 1) % subdivisions) * subdivisions + g;
				int idx3 = idx2 + 1;

				indices.emplace_back(idx0, idx2, idx1);
				indices.emplace_back(idx1, idx2, idx3);
			}
		}
	}

	Init();
}
void SphereObject::Draw(unsigned int shader_program)
{
	glBindVertexArray(VAO);
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(position));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader_program, "material.diffuse"), 1, glm::value_ptr(glm::vec3(0.5, 0.f, 0.f)));
	glUniform3fv(glGetUniformLocation(shader_program, "material.ambient"), 1, glm::value_ptr(glm::vec3(1.f)));
	glUniform3fv(glGetUniformLocation(shader_program, "material.specular"), 1, glm::value_ptr(glm::vec3(0.2f)));
	glUniform1f(glGetUniformLocation(shader_program, "material.shininess"), 32.f);
	glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()*3), GL_UNSIGNED_INT, nullptr);
}
void SphereObject::UpdatePos(glm::vec3 new_pos)
{
	position = new_pos;
}
void SphereObject::Init()
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
