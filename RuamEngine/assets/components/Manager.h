#pragma once

#include "Component.h"
#include "Scene.h"
#include "SceneManager.h"
#include "imgui.h"

#include <iostream>

namespace RuamEngine
{
	class Manager : public Component {
	public:
		using Component::Component;
		void update()
		{
			ImGuiIO& io = ImGui::GetIO();
			ImGui::Text("FPS: %.1f", io.Framerate);
			for (int i = 0; i < SceneManager::sceneList().size(); i++)
			{
				if (ImGui::Button(std::to_string(i).c_str()))
				{
					std::cout << "Escena cambiada wacho\n";
					SceneManager::EnqueueSceneChange(i);
				}
			}
		}
	};
}
