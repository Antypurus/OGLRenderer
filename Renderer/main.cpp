#include <stdio.h>
#include <GL/glew.h>
#include "Core/Window.h"
#include "Core/Shader.h"
#include "Core/VertexBuffer.h"
#include "Core/IndexBuffer.h"
#include "Core/Texture.h"

#include <iostream>
#include "Transform.h"
#include "Animation/Animation.h"

int main()
{
	
    Window window = {800,800,"Window"};
	glewInit(); // Use GLEW to load modern OpenGL functions from the GPU driver, this must be done after creating the window.
	std::vector<Vertex> vertices = {
		{glm::vec3{0.5f,  0.5f, 0.0f},  glm::vec2{1.0f, 1.0f}},  // top right
		{glm::vec3{0.5f, -0.5f, 0.0f},  glm::vec2{1.0f, 0.0f}},  // bottom right
		{glm::vec3{-0.5f, -0.5f, 0.0f}, glm::vec2{0.0f, 0.0f}},  // bottom left
		{glm::vec3{-0.5f,  0.5f, 0.0f}, glm::vec2{0.0f, 1.0f}}   // top left 
	};
	
	std::vector<uint32_t> indices = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	
	VertexBuffer<Vertex> vbo(vertices);
	IndexBuffer ib(indices);
	
	Shader vs = {"./Resources/Shaders/base_vert.glsl", ShaderType::Vertex};
	Shader fs = {"./Resources/Shaders/base_frag.glsl", ShaderType::Fragment};
	GPUProgram shader = {vs, fs};
	
	Texture texture = {"./Resources/Textures/test.jpg"};
	
	Transform model_matrix = {
		glm::vec3{0.0f,0.0f,0.0f},
		glm::vec3{1.0f,1.0f,1.0f},
		glm::vec3{0.0f,0.0f,0.0f}
	};

	Animation animation;

	animation.AddKeyframe(
		{1000000,
		{
			glm::vec3{0.0f,0.0f,0.0f},
			glm::vec3{0.0f,0.0f,0.0f},
			glm::vec3{0.0f,0.0f,90.0f}
		}
		});

	animation.Play();

	while(window.IsOpen())
	{	
		window.ClearViewport();
		Transform final_model = model_matrix + animation.current_transform;
		final_model.Bind("model_matrix",shader);
		shader.Bind();
		texture.Bind();
		vbo.Bind();
		ib.Bind();
		ib.Draw();
		
		window.Update();
	}
	
	return 0;
}
