#pragma once

#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Entity.hpp"
#include <Component.hpp>

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

	public:
		IMPL_SIMPLE_SERIALIZE(Portal)
		using Component::Component;
		Portal(const nlohmann::json& j, unsigned int obj_id) : Component(obj_id) {}
		~Portal()
		{
		}

		void update()
		{
		    if (Camera::GetMainCamera() == nullptr) return;
			if (glm::length(Camera::GetMainCamera()->entity()->transform().position() - entity()->transform().position()) < 1.5f)
			{
			    SceneManager::EnqueueSceneChange(1);
			}
		}
		void start()
		{

		}
	};
	REGISTER_COMPONENT(Portal)
}
