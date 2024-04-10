#include "Core.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
GLFWwindow* Core::Init(int width, int height, const char* window_name, void(*func)(GLFWwindow* window, double xpos, double ypos))
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(width, height, window_name, nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, func);
	return window;
}
