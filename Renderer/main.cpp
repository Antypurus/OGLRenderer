#include<stdio.h>
#include <GL/glew.h>
#include "Core/Window.h"
#include "Core/Shader.h"
#include "Core/VertexBuffer.h"

#include <iostream>

int main()
{
	
    Window window = {800,850,"Window"};
	glewInit(); // Use GLEW to load modern OpenGL functions from the GPU driver, this must be done after creating the window.
	std::vector<Vertex> vertices = {
		// first triangle
		glm::vec3{ 0.5f,  0.5f, 0.0f},  // top right
		glm::vec3{0.5f, -0.5f, 0.0f},  // bottom right
		glm::vec3{-0.5f,  0.5f, 0.0f},  // top left 
		// second triangle
		glm::vec3{ 0.5f, -0.5f, 0.0f},  // bottom right
		glm::vec3{-0.5f, -0.5f, 0.0f},  // bottom left
		glm::vec3{-0.5f,  0.5f, 0.0f}   // top left
	};
	
	VertexBuffer<Vertex> vbo(vertices);
	
	Shader vs = {"./Resources/Shaders/base_vert.glsl", ShaderType::Vertex};
	Shader fs = {"./Resources/Shaders/base_frag.glsl", ShaderType::Fragment};
	GPUProgram shader = {vs, fs};
	
    while(window.IsOpen())
    {	
		window.ClearViewport();

		shader.Bind();
		vbo.Bind();
		vbo.Draw();
		
		window.Update();
    }
    
    return 0;
}
