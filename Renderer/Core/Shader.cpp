#include "Shader.h"
#include <fstream>

Shader::Shader(const std::string& shader_source_path,  const ShaderType& shader_type)
{
	//read shader source
	std::ifstream file_read(shader_source_path);
	std::string source;
	std::string line;
	//reads the glsl source code
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
	
	// creates shader objects and copies overr the source code
	this->shader_object = glCreateShader(static_cast<GLenum>(shader_type));
	const char* shader_program_source = source.c_str();
	glShaderSource(shader_object, 1, &shader_program_source, NULL);
	glCompileShader(shader_object);
	
	
	// check for errors
	int  success;
	char infoLog[512];
	glGetShaderiv(shader_object, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(shader_object, 512, NULL, infoLog);
		printf("Shader Compilation Failed:%s\n", infoLog);
	}
	
}

GPUProgram::GPUProgram(const Shader& vertex_shader, const Shader& fragment_shader)
{
	this->gpu_program_object = glCreateProgram();
	
	//links the vertex and pixel shader into a single shader gpu program
	glAttachShader(this->gpu_program_object ,vertex_shader.shader_object);
	glAttachShader(this->gpu_program_object ,fragment_shader.shader_object);
	glLinkProgram(this->gpu_program_object);
	
	// check for errors
	int  success;
	char infoLog[512];
	glGetProgramiv(this->gpu_program_object, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(this->gpu_program_object, 512, NULL, infoLog);
		printf("Shader GPU Program Linking Failed:%s\n", infoLog);
	}
}

GPUProgram::~GPUProgram()
{
	glDeleteShader(this->vertex_shader.shader_object);
	glDeleteShader(this->fragment_shader.shader_object);
}

void GPUProgram::Bind()
{
	glUseProgram(this->gpu_program_object);
}

//finds a "reference" to a glsl uniform that can be use to interact with that uniform data
int GPUProgram::GetUniformLocation(const std::string& uniform_name)
{
	return glGetUniformLocation(this->gpu_program_object, uniform_name.c_str());
}

//writes a 4x4 matrix into the uniform
void GPUProgram::SetMatrix4f(const std::string& uniform_name, const glm::mat4& matrix)
{
	this->Bind();
	glUniformMatrix4fv(GetUniformLocation(uniform_name),1, GL_FALSE, &matrix[0][0]);
}

//writes a 4 element vector into the uniform
void GPUProgram::SetVec4f(const std::string& uniform_name, const glm::vec4& vector)
{
	this->Bind();
	glUniform4fv(GetUniformLocation(uniform_name),1,&vector[0]);
}
