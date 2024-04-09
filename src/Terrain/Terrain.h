#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct Vertex;
class Camera;
class Player;

class Terrain
{
public:



	void CreateTerrain(float resolution, float size);
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	void Draw(unsigned int& shader_program);
	std::pair<bool,float> CheckHeight(glm::vec3 position);
	float BarycentricCalculation(glm::vec3 p_1, glm::vec3 p_2, glm::vec3 p_3, glm::vec2 pos);
private:
	unsigned int VAO, VBO, EBO;
	float Size;
	int numPrRow;
	void SetupBuffers();
};
