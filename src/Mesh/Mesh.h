#pragma once
#include "glm/vec3.hpp"
#include <vector>

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
struct Triangle {
	unsigned int A, B, C;
	Triangle(unsigned int a, unsigned int b, unsigned int c) : A(a), B(b), C(c){}
};
class SphereObject {
public:
	SphereObject(glm::vec3 position, int subdivisions);
	void Draw(unsigned int shader_program);
private:
	unsigned int VAO, VBO, EBO;
	void Init();
	glm::vec3 position;
	std::vector<Vertex> vertices;
	std::vector<Triangle> indices;
};
