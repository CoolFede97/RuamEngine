#include <iostream>

#include "RuamEngine.h"

// #include "MenuScene.cpp"
#include "SceneManager.h"
#include "SandboxScene.cpp"
#include "assets/scenes/InitialScene.cpp"
#include "assets/scenes/EndScene.cpp"
using namespace RuamEngine;

int main()
{
	Renderer::Init();
	AudioSystem::init();
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

		SceneManager::AddSceneCreator(2, CreateEndScene);
		SceneManager::AddSceneCreator(1, CreateCFSandboxScene);
		SceneManager::AddSceneCreator(0, CreateInitialScene);
		SceneManager::EnqueueSceneChange(0);

		unsigned int frameCount = 0;

		while (!Renderer::WindowShouldClose())
		{

			SceneManager::ApplyPendingSceneChange();

		    // std::cout << "Frame count: " << frameCount++ << "\n";

			// ImGUI
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Input

			// Time
			Time::Update();

			Renderer::ClearScreen();
			Renderer::BeginBatch();

			EventManager::HandleEvents();

            // std::cout << "Frame count: " << frameCount++ << "\n";
			if (SceneManager::ActiveScene() != nullptr)
			{
				SceneManager::ActiveScene()->update();
			}

			Input::UpdateInput();

			if (!SceneManager::SceneChange())
			{
    			Renderer::EndBatch();
    			Renderer::Draw();
			}
			else std::cout << "No se llamo a draw porque se acaba de cambiar de escena\n";

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

	return 0;
}
