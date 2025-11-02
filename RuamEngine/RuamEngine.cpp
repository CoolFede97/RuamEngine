#include <iostream>

#include "RuamEngine.h"

#include "MenuScene.cpp"
#include "SandboxScene.cpp"
#include "assets/components/Manager.h"
#include "assets/scenes/CollisionSandboxScene.cpp"

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


		
		
		CreateMenuScene();
		CreateCFSandboxScene();

		

		//SceneManager::SetActiveScene(0);

		// Scene* collisionSandboxScene = CreateCollisionSandboxScene();

		unsigned int frameCount = 0;

		while (!Renderer::WindowShouldClose())
		{
			std::cout << "Frame " << frameCount++  << "###############################################" << "\n";

			// ImGUI
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			glfwPollEvents();

			// Input
			
			// Time
			Time::Update();

			Renderer::ClearScreen();
			Renderer::BeginBatch();

			EASY_BLOCK("EventManager");
			EventManager::HandleEvents();
			EASY_END_BLOCK;

			EASY_BLOCK("UpdateScene");
			if (SceneManager::ActiveScene() != nullptr)
			{
				SceneManager::SetSceneChange(false);
				SceneManager::ActiveScene()->update();
			}

			Renderer::EndBatch();
			Renderer::Draw();

			EASY_END_BLOCK;
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			Renderer::EndDraw();
			Input::UpdateInput();
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
