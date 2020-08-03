#include <stdio.h>
#include <GL/glew.h>
#include "Core/Window.h"
#include "Core/Shader.h"
#include "Core/VertexBuffer.h"
#include "Core/IndexBuffer.h"
#include "Core/Texture.h"

#include <iostream>

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
	
	while(window.IsOpen())
	{	
		window.ClearViewport();
		
		shader.Bind();
		shader.SetVec4f("u_color",glm::vec4{0.0f,1.0f,0.0f,1.0f});
		texture.Bind();
		vbo.Bind();
		ib.Bind();
		ib.Draw();
		
		window.Update();
	}
	
	return 0;
}
