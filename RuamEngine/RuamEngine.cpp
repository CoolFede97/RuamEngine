#include <iostream>

#include "RuamEngine.h"

#include "assets/scenes/MenuScene.cpp"
#include "assets/scenes/SandboxScene.cpp"
#include "assets/components/Manager.h"

using namespace RuamEngine;

int main()
{
	EASY_PROFILER_ENABLE;

	EASY_BLOCK("System init");
	Renderer::Init();
	AudioSystem::init();
	EASY_END_BLOCK;

	{
		Input::SetWindow(Renderer::GetWindow());
		Input::SetUp(Renderer::GetWindow());
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui_ImplGlfw_InitForOpenGL(Renderer::GetWindow(), true);
		ImGui_ImplOpenGL3_Init();

		ImGui::StyleColorsDark();

		Scene* menuScene = SceneManager::CreateScene(0, "MenuScene");
		SceneManager::AddScene(menuScene);
		
		SceneManager::SetActiveScene(0);
		
		Scene* sandboxScene = SceneManager::CreateScene(1, "SandboxScene");
		SceneManager::AddScene(sandboxScene);

		while (!glfwWindowShouldClose(Renderer::GetWindow()))
		{
			// ImGUI
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// OpenGL
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

			glfwPollEvents();

			// Input
			Input::UpdateInput();

			// Time
			ruamTime::Time::Update();

			Renderer::ClearScreen();
			Renderer::BeginBatch();

			EASY_BLOCK("EventManager");
			EventManager::HandleEvents();
			EASY_END_BLOCK;

			EASY_BLOCK("UpdateScene");
			if (SceneManager::ActiveScene() != nullptr)
			{
				SceneManager::ActiveScene()->update();
			}

			Renderer::EndBatch();
			Renderer::Draw();

			EASY_END_BLOCK;
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			Renderer::EndDraw();

			glfwPollEvents();
		}
	}
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	Renderer::Shutdown();
	AudioSystem::shutdown();

	EASY_PROFILER_DUMP();

	return 0;
}
