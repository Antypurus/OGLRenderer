#include<stdio.h>

#include <GL/glew.h>
#include "Core/Window.h"

int main()
{
    glewInit(); // Use GLEW to load modern OpenGL functions from the GPU driver.
	
    Window window = {800,600,"Window"};

    while(window.IsOpen())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
    }
    
    return 0;
}
