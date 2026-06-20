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
		void update() {
		if (Input::GetKeyDown(KeyCode::SpaceBar_Key))
		{
  Entity* newEntity = SceneManager::ActiveScene()->createEntity("fede"+std::to_string(entityCount));
  entityCount++;
  newEntity->addComponent<ModelRenderer>()->setModel("RuamCore/Assets/Models/Fede.obj");
  newEntity->transform()->setPosition(lastEntityPos+Vec3(4,0,0));
  lastEntityPos = newEntity->transform()->position();
  SceneManager::ActiveScene()->getEntityByName("fede")->transform()->addChild(newEntity->transform());
		}
  // transform()->position().z-=10*RuamTime::DeltaTime();
		};
		IMPL_SIMPLE_SERIALIZE(Sandbox)
	private:
		DECL_REGISTER_COMPONENT(Sandbox)
	};
}
