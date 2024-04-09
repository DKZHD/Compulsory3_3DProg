#include "Player.h"
#include <glad/glad.h>
#include "Mesh.h"
#include <glm/gtc/constants.hpp>
#include <cmath>
#include "../Shader/shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Player::Player(glm::vec3 _position)
{
	position = _position;
	Create(position);
}

glm::vec3 Player::GetVelocity()
{
	return Velocity;
}

glm::vec3 Player::GetPosition()
{
	return position;
}

void Player::UpdateVelocity(glm::vec3 updated_velocity)
{
	Velocity.x = updated_velocity.x;
	Velocity.z = updated_velocity.z;
	Velocity.y = updated_velocity.y;
}

void Player::Update(float deltaTime)
{
	position.y += Velocity.y * deltaTime;
	position.x += Velocity.x * deltaTime;
	position.z += Velocity.z * deltaTime;

	if (IsJumping && Velocity.y == 0.f)
		IsJumping = false;
	if (!IsJumping)
		Velocity.y += Gravity * deltaTime;
	else
		Velocity.y += (Gravity / 3.f) * deltaTime;
}
void Player::UpdatePosition(glm::vec3 updated_pos)
{
	position = updated_pos;
}
void Player::Init()
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

void Player::Create(glm::vec3 position)
{
	float radius = 0.15f;
	int segments = 24;
	for (int i = 0; i < segments; i++)
	{
		float phi = static_cast<float>(i) / (segments-1)* 2.f * glm::pi<float>();
		float y = std::cos(phi) * radius;
		float sphereRadius = std::sin(phi) * radius;

		for (int j = 0; j < segments; j++) {
			float Theta = static_cast<float>(j) / (segments-1)* 2.f * glm::pi<float>();
			float x = sphereRadius * std::sin(Theta);
			float z = sphereRadius * std::cos(Theta);
			if (y <= 0) {
				vertices.emplace_back(glm::vec3(x, y, z));
				vertices.back().Normal = glm::vec3(0.f, 1.f, 0.f);
			}
			else {
				vertices.emplace_back(glm::vec3(x, y+0.4f, z));
				vertices.back().Normal = glm::vec3(0.f, -1.f, 0.f);
			}
				
		}
		int sphereStartIndex = segments+segments;
		for (int i = 0; i < segments - 1; ++i) {
			for (int j = 0; j < segments - 1; ++j) {

				int idx0 = sphereStartIndex+i * segments + j;
				int idx1 = idx0 + 1;
				int idx2 = sphereStartIndex + ((i + 1) % segments)*segments + j;
				int idx3 = idx2 + 1;

				indices.push_back(idx0);
				indices.push_back(idx2);
				indices.push_back(idx1);

				indices.push_back(idx1);
				indices.push_back(idx2);
				indices.push_back(idx3);
			}
		}

	}
	Init();
}
void Player::Draw(unsigned int& shader_program)
{
	glBindVertexArray(VAO);
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(glm::mat4(1.f), glm::vec3(position.x,position.y+0.15f,position.z));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader_program,"material.diffuse"),1, glm::value_ptr(glm::vec3(0.5,0.f,0.f)));
	glUniform3fv(glGetUniformLocation(shader_program,"material.ambient"),1, glm::value_ptr(glm::vec3(1.f)));
	glUniform3fv(glGetUniformLocation(shader_program,"material.specular"),1, glm::value_ptr(glm::vec3(0.2f)));
	glUniform1f(glGetUniformLocation(shader_program,"material.shininess"),32.f);
	glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, nullptr);

}



