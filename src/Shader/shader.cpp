#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "glad/glad.h"


std::pair<std::string, std::string> Shader::LoadShadersFromFile(const char* shader_source_path)
{
	std::ifstream InFile(shader_source_path);
	enum ShaderType
	{
		None, Vertex, Fragment
	};
	if (InFile.is_open())
	{
		std::string line;
		std::stringstream vertexSource;
		std::stringstream fragmentSource;
		ShaderType type = None;
		while (std::getline(InFile, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					type = Vertex;
				else if (line.find("fragment") != std::string::npos)
					type = Fragment;
			}
			else
			{
				switch (type)
				{
				default:
					break;
				case Vertex:
					vertexSource << line << "\n";
					break;
				case Fragment:
					fragmentSource << line << "\n";
					break;
				}
			}
		}
		InFile.close();
		return { vertexSource.str(),fragmentSource.str() };
	}
	return {};
}


void Shader::InitProgram(std::string use_source)
{
	Program = glCreateProgram();
	std::string filePath = "../../../src/Shader/" + use_source;
	const auto shaderSource = LoadShadersFromFile(filePath.c_str());
	const auto charSource = std::make_pair(shaderSource.first.c_str(), shaderSource.second.c_str());
	int success;
	char infoLog[512];
	unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vs, 1, &charSource.first, nullptr);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vs, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glShaderSource(fs, 1, &charSource.second, nullptr);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fs, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glAttachShader(Program, vs);
	glAttachShader(Program, fs);
	glLinkProgram(Program);
	glDeleteShader(vs);
	glDeleteShader(fs);
}

void Shader::Use()
{
	glUseProgram(Program);
}

