/* date = August 2nd 2020 6:21 pm */
#pragma once
#include <vector>

struct IndexBuffer
{
	uint32_t index_buffer_object = 0;
	const std::vector<uint32_t> indices;
	
	public:
	IndexBuffer(const std::vector<uint32_t>& indices);
	void Bind();
	void Draw();
};