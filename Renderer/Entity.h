#pragma once
#include "Transform.h"
#include "Core/IndexBuffer.h"
#include "Core/VertexBuffer.h"
#include "Core/Texture.h"
#include "Core/Shader.h"
#include "Animation/Animation.h"

#include <string>

struct Model
{
	VertexBuffer<Vertex> vertex_buffer;
	IndexBuffer index_buffer;
	Texture texture;

	Model() = default;
	Model(const VertexBuffer<Vertex>& vertex_buffer, const IndexBuffer& index_buffer, const Texture& texture);
	void Bind();
	void Draw();
};

struct Entity
{
	Transform transform;
	Model model;
	Animation animation;
	bool has_animation = false;

	Entity() = default;
	Entity(const Transform& transform, const Model& model);
	Entity(const Transform& transform, const Model& model, const Animation& animation);
	void Draw(GPUProgram& shader, const std::string& model_matrix_uniform_name);
};

