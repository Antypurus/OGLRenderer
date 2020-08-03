/* date = August 3rd 2020 7:30 pm */
#pragma once
#include <glm/glm.hpp>

struct Transform
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	
	Transform(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);
	glm::mat4x4 GenerateModelMatrix();
};