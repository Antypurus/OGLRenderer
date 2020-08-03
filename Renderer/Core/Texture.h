/* date = August 3rd 2020 1:13 am */
#pragma once
#include <memory>
#include <string>

struct Texture
{
	std::unique_ptr<unsigned char> texture_data = nullptr;
	int width = 0;
	int height = 0;
	int channel_count = 0;
	
	Texture(const std::string& texture_path);
};

