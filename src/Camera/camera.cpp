#include "camera.h"
#include "../Shader/shader.h"
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glad/glad.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Mesh/Player.h"

void Camera::Update(Player& player, unsigned int& shader_program)
{
	camPos = player.GetPosition() - camFront * 3.f;
	glm::mat4 proj = glm::perspective(glm::radians(45.f), 16.f / 9.f, 0.1f, 100.f);
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
	glm::mat4 view = glm::lookAt(camPos, player.GetPosition(), glm::vec3(0.f, 1.f, 0.f));
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
}

void Camera::setCamFront(glm::vec3 new_camFront)
{
	camFront = new_camFront;
}
