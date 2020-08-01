/* date = August 1st 2020 5:23 pm */
#pragma once
#include <string>

struct Shader
{
	uint32_t shader_object;
	
	public:
	Shader(const std::string& shader_source_path);
};