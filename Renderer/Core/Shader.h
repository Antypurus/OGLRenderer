/* date = August 1st 2020 5:23 pm */
#pragma once
#include <GL/glew.h>
#include <string>

enum class ShaderType
{
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER,
	Pixel = GL_FRAGMENT_SHADER
};

struct Shader
{
	uint32_t shader_object = 0;
	
	public:
	Shader() = default;
	Shader(const std::string& shader_source_path, const ShaderType& shader_type);
};

struct GPUProgram
{
	uint32_t gpu_program_object = 0;
	Shader vertex_shader;
	Shader fragment_shader;
	
	public:
	GPUProgram(const Shader& vertex_shader, const Shader& fragment_shader);
	~GPUProgram();
	void Bind();
	int GetUniformLocation(const std::string& uniform_name);
};
