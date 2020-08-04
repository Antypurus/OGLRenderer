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

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void InitImGui(const Window& window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window.window_handle, true);

	// defines the glsl version to use by imgui
	ImGui_ImplOpenGL3_Init("#version 330");
}

void ImGuiStartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main()
{
    Window window = {800,800,"Window"};
	
	glewInit(); // Use GLEW to load modern OpenGL functions from the GPU driver, this must be done after creating the window.
	InitImGui(window);
	
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
		{1000,
		{
			glm::vec3{0.0f,0.0f,0.0f},
			glm::vec3{0.0f,0.0f,0.0f},
			glm::vec3{0.0f,0.0f,90.0f}
		}
		});

	animation.AddKeyframe(
		{1000,
		{
			glm::vec3{0.0f,0.0f,0.0f},
			glm::vec3{0.0f,0.0f,0.0f},
			glm::vec3{0.0f,0.0f,-90.0f}
		}
		});

	animation.AddKeyframe(
		{1000,
		{
			glm::vec3{0.0f,0.0f,0.0f},
			glm::vec3{0.0f,0.0f,0.0f},
			glm::vec3{0.0f,0.0f,90.0f}
		}
		});

	animation.AddKeyframe(
		{1000,
		{
			glm::vec3{0.0f,0.0f,0.0f},
			glm::vec3{0.0f,0.0f,0.0f},
			glm::vec3{0.0f,0.0f,0.0f}
		}
		});

	bool show = true;

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
		
		ImGuiStartFrame();

		ImGui::BeginMainMenuBar();
		bool selected = ImGui::Button("test");
		if(selected)
		{
			if(animation.animating)
			{
				if(animation.is_paused)
				{
					animation.Resume();
				}else
				{
					animation.Pause();
				}
			}else
			{
				animation.Play();
			}
		}
		ImGui::EndMainMenuBar();

		ImGuiRender();

		window.Update();
	}

	return 0;
}
