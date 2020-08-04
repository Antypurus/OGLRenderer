#include "Entity.h"

Model::Model(const VertexBuffer<Vertex>& vertex_buffer, const IndexBuffer& index_buffer, const Texture& texture)
	:vertex_buffer(vertex_buffer),index_buffer(index_buffer),texture(texture)
{
}

void Model::Bind()
{
	this->vertex_buffer.Bind();
	this->index_buffer.Bind();
	this->texture.Bind();
}

void Model::Draw()
{
	this->index_buffer.Draw();
}

Entity::Entity(const Transform& transform, const Model& model)
	:transform(transform),model(model)
{
}

Entity::Entity(const Transform& transform, const Model& model, const Animation& animation)
	:transform(transform),model(model),animation(animation),has_animation(true)
{
}

void Entity::Draw(GPUProgram& shader, const std::string& model_matrix_uniform_name)
{
	this->model.Bind();
	if(this->has_animation)
	{
		Transform final_transform = this->transform + animation.current_transform;
		final_transform.Bind(model_matrix_uniform_name, shader);
	} else {
		this->transform.Bind(model_matrix_uniform_name, shader);
	}
	this->model.Draw();
}
