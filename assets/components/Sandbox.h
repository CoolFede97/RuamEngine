#pragma once

#include "Scene.h"
#include "Component.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Transform.h"
#include "RuamTime.h"
#include "Entity.h"
namespace RuamEngine
{
	class Sandbox : public Component
	{
		using Component::Component;

	public:
		void update() {
  transform().position().z-=10*RuamTime::DeltaTime();
		};
		std::string name() override { return "Sandbox"; }
		IMPL_SIMPLE_SERIALIZE(Sandbox)
	private:
		DECL_REGISTER_COMPONENT(Sandbox)
	};
}
