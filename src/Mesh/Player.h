#pragma once
#include <glm/vec3.hpp>
#include <vector>

struct Vertex;

class Player {
public:
	Player(glm::vec3 _position);
	glm::vec3 GetVelocity();
	glm::vec3 GetPosition();
	void UpdateVelocity(glm::vec3 updated_velocity);
	void Update(float deltaTime);
	void UpdatePosition(glm::vec3 updated_pos);
	void Draw(unsigned int& shader_program);

	bool IsJumping = false;

private:
	glm::vec3 Velocity = glm::vec3(0.f);
	float Gravity = -50.f;
	glm::vec3 position;
	void Init();
	void Create(glm::vec3 position);

	unsigned int VAO, VBO, EBO;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};
