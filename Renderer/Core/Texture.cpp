#include "Texture.h"
#include <stb_image.h>
#include <GL/glew.h>

Texture::Texture(const std::string& texture_path)
:texture_data(nullptr, &std::free)
{
	// NOTE(Tiago): load texture data using std_image
	unsigned char* texture_data = stbi_load(texture_path.c_str(), &this->width, &this->height, &this->channel_count, 0);
	
	if(texture_data == nullptr)
	{
		// NOTE(Tiago): failed to read texture
		// TODO(Tiago): how do i want to handle this error without using exceptions
		printf("Unable to read texture data\n");
		return;
	}
	
	// NOTE(Tiago): stb_image texture data must be freed with free and not delete, therefore we have to tell std::unique_ptr to use std::free instead of delete.
	this->texture_data = std::unique_ptr<unsigned char, decltype(&std::free)>(texture_data, &std::free);
	
	// NOTE(Tiago): create texture object
	glGenTextures(1, &this->texture_object);
	// NOTE(Tiago): bind texture
	glBindTexture(GL_TEXTURE_2D, this->texture_object);
	// NOTE(Tiago): set texture filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// NOTE(Tiago): copy texture data to texture object and generate mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->texture_data.get());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture_object);
}
