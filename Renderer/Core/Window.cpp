#include "Window.h"
#include <thread>


Window::Window(uint32_t width, uint32_t height, const std::string& window_name)
:height(height), width(width), name(window_name)
{
    this->CreateWindow();
}

Window::Window(uint32_t width, uint32_t height, std::string&& window_name)
:height(height), width(width), name(std::move(window_name))
{
    this->CreateWindow();
}

Window::~Window()
{
    glfwDestroyWindow(this->window_handle);// NOTE(Tiago): Destroys the window resources
    glfwTerminate();// NOTE(Tiago): Terminate the GLFW library
}

bool Window::IsOpen()
{
    return this->is_open;
}

void Window::CreateWindow()
{
    // NOTE(Tiago): In order to able to interact with a window from another thread we need to create the window on that thread. Since we want to have window event pools be independent from application framerate, we need to create the window in a thread and use that thread to poll its events.
    std::thread window_thread([this](){
								  glfwInit();// NOTE(Tiago):  Initializes the GLFW library
								  
								  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR);// NOTE(Tiago): OpenGL major version
								  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR);// NOTE(Tiago): OpenGL minor version
								  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// NOTE(Tiago): Using OpenGL core features
								  
#if __APPLE__
								  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);// NOTE(Tiago): For MacOS compatibility
#endif
								  
                                  this->window_handle = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr); // NOTE(Tiago): Create the GLFW window
								  
								  if(this->window_handle == nullptr)
								  {
									  //TODO(Tiago):LOG ERROR
									  this->is_open = false;
									  return -1;
								  }
								  
                                  // NOTE(Tiago):  This section will infinitely poll window events and proccess them.
                                  while(this->IsOpen())
                                  {
                                      this->PollEvents();
                                  }
                              });
    window_thread.detach();
}

void Window::PollEvents()
{
    this->is_open = !glfwWindowShouldClose(window_handle);// NOTE(Tiago): Checks if the user asked to close the window
    glfwPollEvents();
}