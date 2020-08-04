/* date = August 3rd 2020 7:30 pm */
#pragma once
#include <string>
#include <glm/glm.hpp>

struct GPUProgram;

struct Transform
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	
	Transform();
	Transform(const Transform& other);
	Transform(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);
	Transform operator+(const Transform& other);
	void operator=(const Transform& other);
	glm::mat4x4 GenerateModelMatrix();
	void Bind(const std::string& uniform_name, GPUProgram& shader);
};