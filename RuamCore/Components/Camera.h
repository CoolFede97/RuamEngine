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
	#define CAMERA_SERIALIZED_MEMBERS(X, ...) \
	X(m_near_plane, float, 0.1f, nullptr)__VA_ARGS__	\
	X(m_far_plane, float, 3000.0f, nullptr)__VA_ARGS__	\
	X(m_fov, float, 45.0f, nullptr)__VA_ARGS__	\
	X(m_aspect_ratio, float, 800.0f/600.0f, nullptr)

	class Camera : public Component
	{
		using Component::Component;

	    static Camera* s_mainCamera;

		const glm::vec3 m_up = glm::vec3(0.0, 1.0, 0.0);

	public:
		CAMERA_SERIALIZED_MEMBERS(DECL_MEMBER)

		IMPL_DRAW_SERIALIZED_MEMBERS(CAMERA_SERIALIZED_MEMBERS(CALL_INSPECTOR_DRAWER))

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
		std::string name() override { return "Camera"; }
		IMPL_SERIALIZE(Camera, CAMERA_SERIALIZED_MEMBERS(SER_FIELD, ,))
	};
}
