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
#include <atomic>
#include "Entity.h"

struct Game
{
	uint64_t credit_count = 0;
	std::atomic_uint64_t play_count = 0;
	uint64_t credits_extracted = 0;
	std::vector<Entity> entities;
	bool animating = false;

	Game()
	{
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
		Texture texture = { "./Resources/Textures/test.jpg" };
		Model model = {vbo, ib, texture};

		Transform model_matrix = {
			glm::vec3{0.0f,0.0f,0.0f},
			glm::vec3{1.0f,1.0f,1.0f},
			glm::vec3{0.0f,0.0f,0.0f}
		};

		constexpr uint64_t entity_count = 10;
		for (uint64_t entity_index = 0; entity_index < entity_count; ++entity_index)
		{
			Animation animation;
			animation.AddKeyframe(
				{ 1000,
				{
					glm::vec3{0.0f,0.0f,0.0f},
					glm::vec3{0.0f,0.0f,0.0f},
					glm::vec3{0.0f,0.0f,90.0f}
				}
				});
			animation.AddKeyframe(
				{ 1000,
				{
					glm::vec3{0.0f,0.0f,0.0f},
					glm::vec3{0.0f,0.0f,0.0f},
					glm::vec3{0.0f,0.0f,-90.0f}
				}
				});
			animation.AddKeyframe(
				{ 1000,
				{
					glm::vec3{0.0f,0.0f,0.0f},
					glm::vec3{0.0f,0.0f,0.0f},
					glm::vec3{0.0f,0.0f,90.0f}
				}
				});
			animation.AddKeyframe(
				{ 1000,
				{
					glm::vec3{0.0f,0.0f,0.0f},
					glm::vec3{0.0f,0.0f,0.0f},
					glm::vec3{0.0f,0.0f,0.0f}
				}
				});

			Entity entity = { model_matrix, model, animation};

			this->entities.push_back(entity);

			//adds the callback only to the last entity
			if (entity_index == entity_count - 1)
			{
				this->entities[this->entities.size() - 1].animation.AddAnimationEndCallback([this]()
					{
						this->animating = false;
						this->play_count++;
					});
			}
		}
	}

	void Start()
	{
		if (this->animating)
		{
			for(Entity& entity:entities)
			{
				entity.animation.PauseResumeToggle();
			}
		}
		else
		{
			if (credit_count > 0)
			{
				this->animating = true;
				credit_count--;
				for (Entity& entity : entities)
				{
					entity.animation.Play();
				}
			}
		}
	}

	void CreditsIn()
	{
		credit_count++;
	}

	void CreditsOut()
	{
		credits_extracted += credit_count;
		credit_count = 0;
	}

	void Draw(GPUProgram& shader, const std::string& model_matrix_uniform_name)
	{
		for (Entity& entity : entities)
		{
			entity.Draw(shader, model_matrix_uniform_name);
		}
	}

};

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
	Window window = { 800,800,"Window" };

	glewInit(); // Use GLEW to load modern OpenGL functions from the GPU driver, this must be done after creating the window.
	InitImGui(window);

	Shader vs = { "./Resources/Shaders/base_vert.glsl", ShaderType::Vertex };
	Shader fs = { "./Resources/Shaders/base_frag.glsl", ShaderType::Fragment };
	GPUProgram shader = { vs, fs };

	Game game;

	while (window.IsOpen())
	{
		window.ClearViewport();
		ImGuiStartFrame();
		ImGui::BeginMainMenuBar();

		bool start_pressed = ImGui::Button("Start");
		if (start_pressed)
		{
			game.Start();
		}

		bool credit_in_pressed = ImGui::Button("Credits In");
		if (credit_in_pressed)
		{
			game.CreditsIn();
		}

		bool credits_out_pressed = ImGui::Button("Credits Out");
		if (credits_out_pressed)
		{
			game.CreditsOut();
		}

		ImGui::EndMainMenuBar();

		ImGui::Begin("Information Board");
		std::string move_count_text = "Number Of Plays:" + std::to_string(game.play_count);
		std::string curr_credits_text = "Current Credits:" + std::to_string(game.credit_count);
		std::string extracted_credits_text = "Extracted Credits:" + std::to_string(game.credits_extracted);
		ImGui::Text(move_count_text.c_str());
		ImGui::Text(curr_credits_text.c_str());
		ImGui::Text(extracted_credits_text.c_str());
		ImGui::End();

		ImGuiRender();

		game.Draw(shader,"model_matrix");

		window.Update();
	}

	return 0;
}
