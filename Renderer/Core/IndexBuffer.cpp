#include "IndexBuffer.h"
#include <GL/glew.h>

IndexBuffer::IndexBuffer(const std::vector<uint32_t>& indices)
:indices(indices)
{
	//create the index buffer objects and write into it the index data
	glGenBuffers(1, &this->index_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), this->indices.data(), GL_STATIC_DRAW);
}

void IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_object);
}

void IndexBuffer::Draw()
{
	// TODO(Tiago): support for index buffer draw offset?
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
}