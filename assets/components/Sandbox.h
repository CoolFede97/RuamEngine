#pragma once

#include "KeyCode.h"
#include "ModelRenderer.h"
#include "Scene.h"
#include "Component.h"
#include "SceneManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Transform.h"
#include "RuamTime.h"
#include "Entity.h"
#include "Input.h"
#include <string>

#include "Renderer.h"
namespace RuamEngine
{
	class Sandbox : public Component
	{
		using Component::Component;

	public:
	    unsigned int entityCount = 0;
	    Vec3 lastEntityPos = {0,0,0};
		void update() override;
		std::vector<FieldInfo> fields() override
		{
            return {makeFieldInfo<unsigned int>("entityCount", entityCount)};
		}
	private:
		DECL_REGISTER_COMPONENT(Sandbox)
	};
}
