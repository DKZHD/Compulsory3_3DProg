#pragma once
#include <vector>
#include <glm/vec3.hpp>
struct Vertex;
class Terrain;
class SphereObject;
enum Direction
{
	FORWARD,
	BACKWARD
};
class Path {
public:
	void GeneratePath(glm::vec3 start_pos, glm::vec3 end_pos, Terrain& terrain);
	void Draw(unsigned int shader_program);
	std::vector<Vertex> GetVertices() {
		return vertices;
	}
	void MoveObject(SphereObject& object, float delta_time, Terrain& terrain);
private:
	std::vector<Vertex> vertices;
	unsigned int VAO, VBO, EBO;
	void SetupBuffer();
	int step = 0;
	float timer = 0.f;
	Direction direction;
};