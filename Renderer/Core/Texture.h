/* date = August 3rd 2020 1:13 am */
#pragma once
#include <memory>
#include <string>
#include <cstdint>
#include <cstdlib>

struct Texture
{
	uint32_t texture_object = 0;
	std::shared_ptr<unsigned char> texture_data;
	int width = 0;
	int height = 0;
	int channel_count = 0;

public:
	Texture() = default;
	Texture(const Texture& other);
	Texture(const std::string& texture_path);
	void Bind();
};

