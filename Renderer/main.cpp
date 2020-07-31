#include<stdio.h>

#include <GL/glew.h>
#include "Core/Window.h"

int main()
{
    glewInit(); // Use GLEW to load modern OpenGL functions from the GPU driver.

    Window window = {800,600,"Window"};
    glViewport(0,0,window.viewport_width, window.viewport_height);

    while(window.IsOpen())
    {
    }
    
    return 0;
}
