#pragma once
#include "glm/vec3.hpp"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 Normal = glm::vec3(0.f);

	glm::vec3 operator-(const Vertex& other) const 
	{
		return this->position - other.position;
	}
	glm::vec3 operator+(const Vertex& other) {
		return this->position + other.position;
	}
};
