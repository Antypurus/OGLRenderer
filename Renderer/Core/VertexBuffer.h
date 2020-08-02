/* date = August 1st 2020 5:04 pm */
#pragma once
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

struct Vertex
{
	glm::vec3 position;
	
	public:
	Vertex(const glm::vec3& position):position(position){};
	void SetVertexAttributes(uint32_t layout_slot = 0)
	{
		glVertexAttribPointer(layout_slot, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	}
};

template<typename vertex_type>
struct VertexBuffer
{
	public:
	uint32_t vertex_buffer_object = 0;
	uint32_t vertex_attribute_array_object = 0;
	std::vector<vertex_type> vertices;
	
	public:
	VertexBuffer(const std::vector<vertex_type>& vertices, uint32_t layout_slot = 0)
		:vertices(vertices)
	{
		//creates the opengl VBO and VAO
		glGenVertexArrays(1, &this->vertex_attribute_array_object);
		glBindVertexArray(this->vertex_attribute_array_object);
		
		//Binds the VAO and VBO
		glGenBuffers(1, &this->vertex_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_object);
		
		//Copies data into the VBO
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex_type), vertices.data(), GL_STATIC_DRAW);
		
		//Records the vertex configuration data into the VAO
		this->vertices[0].SetVertexAttributes(layout_slot);
		
		//enables the VAO
		glEnableVertexAttribArray(layout_slot);
	}
	
	void Bind()
	{
		// NOTE(Tiago): Binds the VBO containing the vertex configuration data for usage
		glBindVertexArray(this->vertex_attribute_array_object);
	}
	
	void Draw()
	{
		// TODO(Tiago): Support for non-triangle-based vbos
		glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	}
};

