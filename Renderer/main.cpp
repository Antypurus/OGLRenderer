#include<stdio.h>

#include <GL/glew.h>
#include "Core/Window.h"

int main()
{
    glewInit(); // Use GLEW to load modern OpenGL functions from the GPU driver.

    Window window = {800,600,"Window"};

    while(window.IsOpen())
    {
        
    }
    
    return 0;
}
