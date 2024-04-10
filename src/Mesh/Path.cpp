#include "Path.h"
#include <glm/vec3.hpp>
#include "Mesh.h"
#include "../Terrain/Terrain.h"
#include <glm/geometric.hpp>
#include <cmath>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


void Path::GeneratePath(glm::vec3 start_pos, glm::vec3 end_pos, Terrain& terrain)
{
	start_pos.y = -1.f;
	start_pos.y = terrain.CheckHeight(start_pos).second + 0.05f;
	end_pos.y = -1.f;
	end_pos.y = terrain.CheckHeight(end_pos).second + 0.05f;
	float length = std::sqrt(std::powf((end_pos.x - start_pos.x),2) + std::powf((end_pos.z - start_pos.z),2));

	float stepSize = 0.08f;
	vertices.emplace_back(start_pos);
	for (float i = 0.f; i < length; i+=stepSize) {
		glm::vec3 vertexPosition = start_pos + ((end_pos - start_pos) *i/length);
		vertexPosition.y = -1.f;
		auto heightPair = terrain.CheckHeight(vertexPosition);
		if(heightPair.first)
		vertexPosition.y = heightPair.second+0.05f;
		vertices.emplace_back(vertexPosition);
		std::cout << "X: " << vertexPosition.x << " Y: " << vertexPosition.y << " Z: " << vertexPosition.z << std::endl;
	}
	vertices.emplace_back(end_pos);
	SetupBuffer();
}

void Path::Draw(unsigned int shader_program)
{
	glBindVertexArray(VAO);
	glm::mat4 model = glm::mat4(1.f);
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader_program, "material.diffuse"), 1, glm::value_ptr(glm::vec3(1.f, 0.f, 0.f)));
	glUniform3fv(glGetUniformLocation(shader_program, "material.ambient"), 1, glm::value_ptr(glm::vec3(1.f)));
	glDrawArrays(GL_LINE_STRIP_ADJACENCY, 0, vertices.size());
}

void Path::MoveObject(SphereObject& object, float delta_time, Terrain& terrain)
{
	timer += delta_time;
	if (timer > 0.01f) {
		if (step+1 >= vertices.size() - 1)
			direction = BACKWARD;
		else if(step<=0)
			direction = FORWARD;
		switch (direction)
		{
		case FORWARD:
			step++;
			break;
		case BACKWARD:
			step--;
			break;
		default:
			break;
		}
		object.UpdatePos(glm::vec3(vertices[step].position.x,
			terrain.CheckHeight(glm::vec3(vertices[step].position.x, -1.f,vertices[step].position.z)).second+0.45,vertices[step].position.z));
		timer = 0.f;
	}

}

void Path::SetupBuffer()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(),GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
}
