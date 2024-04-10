#include "Compulsory3.h"
#include <iostream>

#include "Camera/camera.h"
#include "glad/glad.h"
#include <glm/ext/matrix_clip_space.hpp>
#include "Shader/shader.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp"
#include "Mesh/Mesh.h"
#include "Terrain/Terrain.h"
#include "Mesh/Player.h"
#include "Mesh/Path.h"
#include "Core/Core.h"

float DeltaTime = 0.f;
void Input(GLFWwindow* _window, Camera& _camera, Player& _player)
{
	if(glfwGetKey(_window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
	{
		glfwSetWindowShouldClose(_window, true);
	}
	_player.UpdateVelocity(glm::vec3(0.f,_player.GetVelocity().y, 0.f));
	glm::vec3 tempVelocity = _player.GetVelocity();
	if(glfwGetKey(_window,GLFW_KEY_SPACE)==GLFW_PRESS)
	{
		if (!_player.IsJumping) {
			_player.IsJumping = true;
			tempVelocity.y = 5.f;
		}
	}
	if(glfwGetKey(_window,GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS)
	{
		_camera.camPos.y -= 0.01f;
	}
	if(glfwGetKey(_window,GLFW_KEY_W)==GLFW_PRESS)
	{
		tempVelocity += (glm::vec3(_camera.GetCamFront().x*5.f, 0.f,_camera.GetCamFront().z*5.f));
	}
	if(glfwGetKey(_window,GLFW_KEY_S)==GLFW_PRESS)
	{
		tempVelocity -= (glm::vec3(_camera.GetCamFront().x * 5.f, 0.f, _camera.GetCamFront().z * 5.f));
	}
	if(glfwGetKey(_window,GLFW_KEY_A)==GLFW_PRESS)
	{
		glm::vec3 leftMovement = (glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), _camera.GetCamFront())) * 5.f);
		tempVelocity += (glm::vec3(leftMovement.x, 0.f,leftMovement.z));
	}
	if(glfwGetKey(_window,GLFW_KEY_D)==GLFW_PRESS)
	{
		glm::vec3 rightMovement = (glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), _camera.GetCamFront())) * 5.f);
		tempVelocity += (glm::vec3(-rightMovement.x, 0.f, -rightMovement.z));
	}
	_player.UpdateVelocity(tempVelocity);
}
double lastX, lastY;
float pitch = 0.f, roll = 0.f, yaw = -90.f;
bool FirstMouse = true;
Camera camera;
void MouseMovement(GLFWwindow* window, double xpos, double ypos)
{
	if(FirstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		FirstMouse = false;
	}
	float currentXoffset = static_cast<float>(xpos - lastX);
	float currentYoffset = static_cast<float>(lastY - ypos);

	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f;
	currentXoffset *= sensitivity;
	currentYoffset *= sensitivity;
	yaw += currentXoffset;
	pitch += currentYoffset;
	if (pitch < -89.f)
		pitch = -89.f;
	else if (pitch > 89.f)
		pitch = 89.f;
	glm::vec3 newForward;
	newForward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newForward.y = sin(glm::radians(pitch));
	newForward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera.setCamFront(glm::normalize(newForward));
}
int main()
{
	GLFWwindow* window = Core::Init(1280,720,"Compulsory 3",MouseMovement);
	Shader defaultShader;
	defaultShader.InitProgram("shaderSource.shader");
	defaultShader.Use();

	Terrain terrain;
	terrain.CreateTerrain(5.f, 50.f);

	Path path;
	path.GeneratePath(glm::vec3(3.f,0.f,3.f),glm::vec3(30.f,0.f,30.f),terrain);

	Player player(glm::vec3(3.f,2.f,3.f));

	SphereObject sphere(glm::vec3(5.f, 0.f, 5.f), 32);
	SphereObject sphereOnSurface(glm::vec3(8.f, terrain.CheckHeight(glm::vec3(8.f, 0.f, 10.f)).second-0.45f, 10.f), 32);

	Shader shadowShader;
	shadowShader.InitProgram("depthSource.shader");
	shadowShader.Use();
	unsigned int depthMapFBO;
	glGenFramebuffers(1,&depthMapFBO);

	const unsigned int Shadow_Height = 4096, Shadow_Width = 4096;

	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Shadow_Width, Shadow_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		DeltaTime = currentTime - lastTime;
		lastTime = currentTime;
		/*std::cout << DeltaTime << "\n";*/
		/*std::cout << "FPS: " << 1.f / DeltaTime << "\n";*/

		float near = 1.f, far = 50.f;
		glm::mat4 lightProj = glm::ortho(-25.f, 25.f,-25.f,25.f,near, far);
		glm::mat4 lightView = glm::lookAt(glm::vec3(-5.f, 5.f, 10.f), glm::vec3(5.f,0.f,15.f), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 LightMatrix = lightProj * lightView;
		shadowShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "lightMatrix"), 1, GL_FALSE, glm::value_ptr(LightMatrix));
		glViewport(0, 0, Shadow_Width, Shadow_Height);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		player.Draw(shadowShader.Program);
		terrain.Draw(shadowShader.Program);
		sphere.Draw(shadowShader.Program);
		sphereOnSurface.Draw(shadowShader.Program);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, 1280, 720);
		glClearColor(0.f, 0.7f, 0.7f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		defaultShader.Use();
		camera.Update(player, defaultShader.Program);
		glUniform3fv(glGetUniformLocation(defaultShader.Program, "viewPos"), 1, glm::value_ptr(camera.camPos));
		glUniform3fv(glGetUniformLocation(defaultShader.Program, "lightPos"), 1, glm::value_ptr(glm::vec3(0.f, 3.f, 6.f)));
		glUniformMatrix4fv(glGetUniformLocation(defaultShader.Program, "lightMatrix"), 1, GL_FALSE, glm::value_ptr(LightMatrix));
		auto height = terrain.CheckHeight(player.GetPosition());
		if (height.first) {
			player.UpdateVelocity(glm::vec3(player.GetVelocity().x,0.f,player.GetVelocity().z));
			player.UpdatePosition(glm::vec3(player.GetPosition().x, height.second, player.GetPosition().z));
		}
		player.Draw(defaultShader.Program);
		path.MoveObject(sphere, DeltaTime, terrain);
		sphere.Draw(defaultShader.Program);
		sphereOnSurface.Draw(defaultShader.Program);
		path.Draw(defaultShader.Program);
		Input(window, camera, player);
		player.Update(DeltaTime);
		
		terrain.Draw(defaultShader.Program);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
