#include "Shader.h"
#include <fstream>

Shader::Shader(const std::string& shader_source_path,  const ShaderType& shader_type)
{
	//read shader source
	std::ifstream file_read(shader_source_path);
	std::string source;
	std::string line;
	if (file_read.is_open())
	{
		while (std::getline(file_read, line))
		{
			source += line + '\n';
		}
	} else {
		printf("Failed to open shader source file");
		return;
	}
	
	this->shader_object = glCreateShader(static_cast<GLenum>(shader_type));
	const char* shader_program_source = source.c_str();
	glShaderSource(shader_object, 1, &shader_program_source, NULL);
	glCompileShader(shader_object);
	
	int  success;
	char infoLog[512];
	glGetShaderiv(shader_object, GL_COMPILE_STATUS, &success);
	
	if(!success)
	{
		glGetShaderInfoLog(shader_object, 512, NULL, infoLog);
		printf("Shader Compilatio Failed:%s\n", infoLog);
	}
	
}