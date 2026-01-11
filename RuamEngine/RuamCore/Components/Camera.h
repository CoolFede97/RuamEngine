#pragma once

#include "SceneManager.h"
#include "Scene.h"
#include "Entity.h"
//#include "Serial.h"
#include "Component.h"
#include "AudioSystem.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Transform.h"

namespace RuamEngine
{
	class Camera : public Component
	{
		using Component::Component;

		static Camera* s_mainCamera;

		const glm::vec3 m_up = glm::vec3(0.0, 1.0, 0.0);

	public:
		float m_near_plane = 0.1f;
		float m_far_plane = 3000.0f;
		float m_fov = 45.0f;
		float m_aspect_ratio = 800.0f/600.0f;

		glm::mat4 projectionMatrix();
		glm::mat4 viewMatrix();

		void setAsMainCamera();
		static void EmptyMainCamera();
		static Camera* GetMainCamera();

		void update() {
			Component::update();
			AudioSystem::AL::Listener::setParam(AL_POSITION, entity()->transform().position());
		};
		void start() { setAsMainCamera();};
	};
}
