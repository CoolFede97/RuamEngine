#pragma once

#include "Component.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "imgui.h"

#include <iostream>

class Manager : public BaseRenderer {
public:
	using BaseRenderer::BaseRenderer;
	Manager(const nlohmann::json& j, int obj_id) : BaseRenderer(obj_id) {};
	void render() {
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("FPS: %.1f", io.Framerate);
		int i = 0;
		for (const auto& scene : SceneManager::sceneList()) 
		{
			if (ImGui::Button(std::to_string(i).c_str()))
			{
				SceneManager::SetActiveScene(i);
			}
			i++;
		}
	};
	void update()
	{
		BaseRenderer::update();
	}

	IMPL_SIMPLE_SERIALIZE(Manager)
};

REGISTER_COMPONENT(Manager)
