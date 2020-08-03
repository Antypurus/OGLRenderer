#include "Transform.h"
#include <glm/gtx/transform.hpp>

#include "Core/Shader.h"

using namespace glm;

Transform::Transform()
:position(vec3{0,0,0}),scale(vec3{1,1,1,}),rotation(vec3{0,0,0})
{
}

Transform::Transform(vec3 position, vec3 scale, vec3 rotation)
:position(position), scale(scale), rotation(rotation)
{
}

mat4 Transform::GenerateModelMatrix()
{
	mat4 results = glm::mat4();
	results *= translate(this->position);
	//results *= glm::scale(this->scale);
	//NOTE(Tiago): rotate in each of the axis
	//results *= rotate(this->rotation[0], vec3{1,0,0});
	//results *= rotate(this->rotation[1], vec3{0,1,0});
	//results *= rotate(this->rotation[2], vec3{0,0,1});
	//results *= translate(-this->position);
	return results;
}

void Transform::Bind(const std::string& uniform_name, GPUProgram& shader)
{
	shader.SetMatrix4f(uniform_name, this->GenerateModelMatrix());
}