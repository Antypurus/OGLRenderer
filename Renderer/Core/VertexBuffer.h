/* date = August 1st 2020 5:04 pm */
#pragma once
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	
	public:
	Vertex(const glm::vec3& position):position(position){};
	void SetVertexAttributes(uint32_t layout_slot = 0)
	{
		glVertexAttribPointer(layout_slot, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(layout_slot);
	}
};

template<typename vertex_type>
struct VertexBuffer
{
	public:
	uint32_t vertex_buffer_object = 0;
	std::vector<vertex_type> vertices;
	
	public:
	VertexBuffer(const std::vector<vertex_type>& vertices)
		:vertices(vertices)
	{
		glGenBuffers(1, &this->vertex_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_object);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex_type), vertices.data(), GL_STATIC_DRAW);
		//this->vertices[0].SetVertexAttributes();
	}
};

