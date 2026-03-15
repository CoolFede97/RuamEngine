#pragma once

#include "Component.h"
#include "Scene.h"
#include "SceneManager.h"
#include "imgui.h"

#include <iostream>

namespace RuamEngine
{
	class Manager : public Component
	{
	public:
		using Component::Component;

		IMPL_SIMPLE_SERIALIZE(Manager)
		std::string name() override { return "Manager"; }
		IMPL_DRAW_SERIALIZED_MEMBERS(;)

		void update() override
		{
			ImGuiIO& io = ImGui::GetIO();
			ImGui::Text("FPS: %.1f", io.Framerate);
			for (int i = 0; i < SceneManager::scenes().size(); i++)
			{
				if (ImGui::Button(std::to_string(i).c_str()))
				{
					std::cout << "Escena cambiada wacho\n";
					// SceneManager::EnqueueSceneChange(i);
				}
			}
		}
	};
}
