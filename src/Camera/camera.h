#pragma once
#include "glm/vec3.hpp"

class Player;

class Camera
{
public:
	glm::vec3 camPos = glm::vec3(0.f,0.f,5.f);
	void Update(Player& player, unsigned int& shader_program);
	void setCamFront(glm::vec3 new_camFront);
	glm::vec3 GetCamFront() const { return camFront; }
private:
	glm::vec3 camFront = glm::vec3(0.f, 0.f, -1.f);
};