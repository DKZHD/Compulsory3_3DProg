#pragma once
#include <string>

class Shader
{
	static std::pair<std::string, std::string>LoadShadersFromFile(const char* shader_source_path);
public:
	unsigned int Program;
	void InitProgram(std::string use_source);
	void Use();
};
