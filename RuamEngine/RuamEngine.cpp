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
		EASY_BLOCK("Imgui init");
		Input::SetWindow(Renderer::GetWindow());
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(Renderer::GetWindow(), true);
		ImGui::StyleColorsDark();
		EASY_END_BLOCK;

		EASY_BLOCK("Scene adding");
		const unsigned int menuScene = SceneManager::AddScene(0, CreateMenuScene);
		SceneManager::SetActiveScene(menuScene);
		const unsigned int sandboxScene = SceneManager::AddScene(1, CreateSandboxScene);
		EASY_END_BLOCK;

		EASY_BLOCK("Loop");
		while (!glfwWindowShouldClose(Renderer::GetWindow()))
		{
			// ImGUI
			ImGui_ImplGlfwGL3_NewFrame();

			// OpenGL
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

			glfwPollEvents();

			// Input
			Input::UpdateInput();

			// Time
			ruamTime::Time::Update();

			Renderer::BeginDraw();

			EASY_BLOCK("EventManager");
			EventManager::HandleEvents();
			EASY_END_BLOCK;

			EASY_BLOCK("UpdateScene");
			if (SceneManager::ActiveScene() != nullptr)
			{
				SceneManager::ActiveScene()->update();
			}
			EASY_END_BLOCK;
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			Renderer::EndDraw();

			glfwPollEvents();
		}
		EASY_END_BLOCK;
	}
	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	Renderer::Shutdown();
	AudioSystem::shutdown();

	EASY_PROFILER_DUMP();

	return 0;
}
