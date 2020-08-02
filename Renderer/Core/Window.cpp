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

void Window::SetOpenGLViewport(uint32_t x_offset, uint32_t y_offset)
{
	// TODO(Tiago): Log viewport creation
	glViewport(x_offset, y_offset, this->viewport_width, this->viewport_height);
}

void Window::MakeContextCurrent()
{
	// TODO(Tiago): Log context switch
	glfwMakeContextCurrent(this->window_handle); 
}

void Window::MakeContextNonCurrent()
{
	// TODO(Tiago): log context switch
	glfwMakeContextCurrent(nullptr);
}

void Window::CreateWindow()
{
	glfwInit();// NOTE(Tiago):  Initializes the GLFW library
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR);// NOTE(Tiago): OpenGL major version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR);// NOTE(Tiago): OpenGL minor version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// NOTE(Tiago): Using OpenGL core features
	
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);// NOTE(Tiago): For MacOS compatibility
#endif
	
	this->window_handle = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr); // NOTE(Tiago): Create the GLFW window
	// TODO(Tiago): Log window creation attempt and status
	
	this->MakeContextNonCurrent();
	
	if(this->window_handle == nullptr)
	{
		//TODO(Tiago):LOG ERROR
		this->is_open = false;
		this->failed_to_open= true;
		return;
	} else {
		this->is_open = true;
	}
	
	// NOTE(Tiago): Determine size of the viewport region after window creation
	glfwGetFramebufferSize(this->window_handle, &this->viewport_width, &this->viewport_height);
	this->RegisterViewportResizeCallback();
	
	if(!this->failed_to_open)
	{
		this->Update();
		this->SetOpenGLViewport();
		this->MakeContextCurrent();
	}
}

void Window::PollEvents()
{
    this->is_open = !glfwWindowShouldClose(window_handle);// NOTE(Tiago): Checks if the user asked to close the window
    glfwPollEvents();
}

// NOTE(Tiago): Actual viewport resize callback function
static void viewport_resize_callback(GLFWwindow* window, int width, int height)
{
	// TODO(Tiago): Figure out a way to update the underlying window class data from here (global map window ptr -> window class maybe?)
	int viewport_width;
	int viewport_height;
	glfwGetFramebufferSize(window, &viewport_width, &viewport_height);
	glViewport(0, 0, viewport_width, viewport_height);
};

void Window::RegisterViewportResizeCallback()
{
	// NOTE(Tiago): has to be used in the same thread as the thread that handles window creation and even polling
	glfwSetFramebufferSizeCallback(this->window_handle, viewport_resize_callback);
}

void Window::Update()
{
	glfwSwapBuffers(this->window_handle);
	this->PollEvents();
}

void Window::ClearViewport()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::ClearViewport(float clear_color[4])
{
	glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
	glClear(GL_COLOR_BUFFER_BIT);
}

