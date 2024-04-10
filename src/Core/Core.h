#pragma once
struct GLFWwindow;
class Core 
{
public:
	static GLFWwindow* Init(int width, int height, const char* window_name, void(*func)(GLFWwindow* window, double xpos, double ypos));
};