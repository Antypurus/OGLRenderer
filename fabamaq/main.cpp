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
		// square that makes up a "piece"
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

		// 4 models that server to randomize appearances
		Texture texture1 = { "./Resources/Textures/tex1.png" };
		Texture texture2 = { "./Resources/Textures/tex2.jpg" };
		Texture texture3 = { "./Resources/Textures/tex3.jpg" };
		Texture texture4 = { "./Resources/Textures/tex4.png" };
		
		Model model1 = {vbo, ib, texture1};
		Model model2 = {vbo, ib, texture2};
		Model model3 = {vbo, ib, texture3};
		Model model4 = {vbo, ib, texture4};

		Model models[] = {model1,model2,model3,model4};

		// base transform of each "piece"
		Transform model_matrix = {
			glm::vec3{-0.8f,-0.8f,0.0f},
			glm::vec3{0.15f,0.15f,1.0f},
			glm::vec3{0.0f,0.0f,0.0f}
		};

		constexpr uint64_t entity_count = 64;
		for (uint64_t entity_index = 0; entity_index < entity_count; ++entity_index)
		{
			const uint64_t time_delta = entity_index*20;
			// constructs an animation where each keyframe has a start latency based on its index so as to interleave piece animations
			Animation animation;
			// what we do here is add a keyframe that does nothing until its time for the piece to start its animation
			animation.AddKeyframe({ 0 + time_delta,
				{
					glm::vec3{0.0f,0.0f,0.0f},
					glm::vec3{0.0f,0.0f,0.0f},
					glm::vec3{0.0f,0.0f,0.0f}
				}
				});
			animation.AddKeyframe(
				{ 1000 + time_delta,
				{
					glm::vec3{0.0f,0.0f,0.0f},
					glm::vec3{0.0f,0.0f,0.0f},
					glm::vec3{0.0f,0.0f,90.0f}
				}
				});
			animation.AddKeyframe(
				{ 1000 + time_delta,
				{
					glm::vec3{0.0f,0.0f,0.0f},
					glm::vec3{0.0f,0.0f,0.0f},
					glm::vec3{0.0f,0.0f,-90.0f}
				}
				});
			animation.AddKeyframe(
				{ 1000 + time_delta,
				{
					glm::vec3{0.0f,0.0f,0.0f},
					glm::vec3{0.0f,0.0f,0.0f},
					glm::vec3{0.0f,0.0f,90.0f}
				}
				});
			animation.AddKeyframe(
				{ 1000 + time_delta,
				{
					glm::vec3{0.0f,0.0f,0.0f},
					glm::vec3{0.0f,0.0f,0.0f},
					glm::vec3{0.0f,0.0f,0.0f}
				}
				});

			// compute the position on the screen where a piece should be based on its index
			float separation_scalar = 1.5f;
			float x = (((uint64_t)(entity_index / 8.0f) % 8) / 8.0f) * separation_scalar;
			float y = ((entity_index % 8) / 8.0f) * separation_scalar;

			// transform that will displace the piece from its base position
			Transform delta_transform = {
				glm::vec3{ x, y, 0},
				glm::vec3{0,0,0},
				glm::vec3{0,0,0}
			};

			Entity entity = { model_matrix + delta_transform, models[entity_index % 4], animation};
			this->entities.push_back(entity);

			//adds the callback only to the last entity, so that we know when all pieces are done animating
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

	// function that is called when the start button is pressed
	void Start()
	{
		if (this->animating)
		{
			// if animation is underway pause or resume the animation
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
				// if the user has enough credits to play, start animating every piece
				for (Entity& entity : entities)
				{
					entity.animation.Play();
				}
			}
		}
	}

	// function that is called when the credits in button is pressed
	void CreditsIn()
	{
		credit_count++;
	}

	// function that is called when the credits out button is pressed
	void CreditsOut()
	{
		credits_extracted += credit_count;
		credit_count = 0;
	}

	// updates piece animation and draws the pieces
	void Draw(GPUProgram& shader, const std::string& model_matrix_uniform_name)
	{
		for (Entity& entity : entities)
		{
			entity.animation.Update();
			entity.Draw(shader, model_matrix_uniform_name);
		}
	}

};

// initializes the imgui interface
void InitImGui(const Window& window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window.window_handle, true);

	// defines the glsl version to use by imgui
	ImGui_ImplOpenGL3_Init("#version 330");
}

// starts a new imgui frame
void ImGuiStartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

// renders the imgui interface
void ImGuiRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main()
{
	Window window = { 800,800,"Fabamaq Take Home Test" };

	glewInit(); // Use GLEW to load modern OpenGL functions from the GPU driver, this must be done after creating the window.
	InitImGui(window);

	// create the sahder that will run
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

		// display information about game state
		ImGui::Begin("Information Board");
		std::string move_count_text = "Number Of Plays:" + std::to_string(game.play_count);
		std::string curr_credits_text = "Current Credits:" + std::to_string(game.credit_count);
		std::string extracted_credits_text = "Extracted Credits:" + std::to_string(game.credits_extracted);
		ImGui::Text(move_count_text.c_str());
		ImGui::Text(curr_credits_text.c_str());
		ImGui::Text(extracted_credits_text.c_str());
		ImGui::End();

		game.Draw(shader,"model_matrix");
		ImGuiRender();

		window.Update();
	}

	return 0;
}
