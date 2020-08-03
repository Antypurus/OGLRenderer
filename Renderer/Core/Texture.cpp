#include "Texture.h"
#include <stb_image.h>

Texture::Texture(const std::string& texture_path)
{
	unsigned char* texture_data = stbi_load(texture_path.c_str(), &this->width, &this->height, &this->channel_count, 0);
	this->texture_data = std::unique_ptr<unsigned char>(texture_data);
}

