#pragma once

#include "SceneManager.h"
#include "Scene.h"
#include "Entity.h"
#include "Component.h"

#include "ShaderProgram.h"
#include "Renderer.h"

#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shooter.h"

namespace RuamEngine
{
	class Portal : public Component
	{
		using Component::Component;

	public:
		IMPL_SIMPLE_SERIALIZE(Portal)
		IMPL_DRAW_SERIALIZED_MEMBERS(;)
		std::string name() override { return "Portal"; }

		~Portal()
		{
		}

		void update()
		{
		    if (Camera::GetMainCamera() == nullptr) return;
			if (glm::length(Camera::GetMainCamera()->entity()->transform().position() - entity()->transform().position()) < 1.5f)
			{
			    SceneManager::EnqueueSceneChange("SandboxScene");
			}
		}

		void start()
		{

		}
	};
}
