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
	uint32_t shader_object;
	
	public:
	Shader(const std::string& shader_source_path, const ShaderType& shader_type);
};