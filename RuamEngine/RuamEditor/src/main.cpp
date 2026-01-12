#include <iostream>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "SceneManager.h"
#include "SandboxScene.cpp"
#include "../../assets/scenes/InitialScene.cpp"

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

			static Entity* selectedEntity = nullptr;

			ImGui::Begin("Hierarchy");
			Scene* scene = SceneManager::ActiveScene();
			if (scene != nullptr)
			{
				if (ImGui::Button("Create Entity"))
				{
					selectedEntity = scene->createEntity();
				}

				ImGui::SameLine();

				if (selectedEntity != nullptr)
				{
					if (ImGui::Button("Delete selected entity"))
					{
						selectedEntity->destroy();
						selectedEntity = nullptr;
					}
				}

				ImGui::Separator();

				std::list<Entity*> entities = scene->getEntities();
				for (Entity* entity : entities)
				{
					bool selected = (selectedEntity == entity);
					if (ImGui::Selectable(entity->name().c_str(), selected))
					{
						selectedEntity = entity;
					}
				}
			}
			ImGui::End();
			// Input

			// Time
			Time::Update();

			Renderer::ClearScreen();
			Renderer::BeginBatch();

			EventManager::HandleEvents();

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
