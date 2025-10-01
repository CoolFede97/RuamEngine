#include <iostream>

#include "RuamEngine.h"

#include "assets/scenes/MenuScene.cpp"
#include "assets/scenes/SandboxScene.cpp"
#include "assets/components/Manager.h"
#include "easy/profiler.h"

using namespace RuamEngine;

int main()
{
	EASY_PROFILER_ENABLE;

	profiler::startListen();

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

		const unsigned int menuScene = SceneManager::AddScene(0, CreateMenuScene);
		SceneManager::SetActiveScene(menuScene);
		const unsigned int sandboxScene = SceneManager::AddScene(1, CreateSandboxScene);

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

	profiler::dumpBlocksToFile("log.perf");
	return 0;
}
