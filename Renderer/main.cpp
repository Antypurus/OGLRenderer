#include<stdio.h>

#include <GL/glew.h>
#include "Core/Window.h"
#include "Core/Shader.h"

#include <iostream>

int main()
{
	
    Window window = {800,600,"Window"};
    
	glewInit(); // Use GLEW to load modern OpenGL functions from the GPU driver, this must be done after creating the window.
	
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	
	unsigned int vbo;
	glGenBuffers(1,&vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	Shader vs = {"./Resources/Shaders/base.glsl", ShaderType::Vertex};
	
    while(window.IsOpen())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
    }
    
    return 0;
}
