#pragma once

#include "Component.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "imgui.h"

#include <iostream>

namespace RuamEngine
{
	class Manager : public BaseRenderer {
	public:
		using BaseRenderer::BaseRenderer;
		Manager(const nlohmann::json& j, int obj_id) : BaseRenderer(obj_id) {};
		void render() {
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
		};
		void update()
		{
			BaseRenderer::update();
		}

		IMPL_SIMPLE_SERIALIZE(Manager)
	};

	REGISTER_COMPONENT(Manager)
}
