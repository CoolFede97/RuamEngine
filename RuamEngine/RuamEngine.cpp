#include <iostream>

#include "RuamEngine.h"

#include "assets/scenes/MenuScene.cpp"
#include "assets/scenes/SandboxScenes.cpp"
#include "assets/components/Manager.h"

using namespace RuamEngine;

int main()
{
	Renderer::Init();
	
	{
		Input::SetWindow(Renderer::GetWindow());
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(Renderer::GetWindow(), true);
		ImGui::StyleColorsDark();

		const unsigned int menuScene = SceneManager::AddScene(0, CreateMenuScene);
		SceneManager::SetActiveScene(menuScene);
		const unsigned int sandboxScene = SceneManager::AddScene(1, CreateSandboxScene);
		const unsigned int sandboxScene2 = SceneManager::AddScene(2, CreateSandboxScene2);

		int frameCount = 0;


		while (!glfwWindowShouldClose(Renderer::GetWindow()))
		{
			//std::cout << "Frame: " << frameCount++ << "---------------------------------------------------------\n";
			// ImGUI
			ImGui_ImplGlfwGL3_NewFrame();

			// Input
			Input::UpdateInput();

			// Time
			ruamTime::Time::Update();

			Renderer::ClearScreen();
			Renderer::BeginBatch();

			if (SceneManager::ActiveScene() != nullptr)
			{
				SceneManager::ActiveScene()->update();
			}

			Renderer::EndBatch();
			Renderer::Draw();
			
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			Renderer::EndDraw();
			glfwPollEvents();
		}
	}
	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	Renderer::Shutdown();

	return 0;
}
