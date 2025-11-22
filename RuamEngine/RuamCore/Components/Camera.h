#pragma once

#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Object.hpp"
#include "Serial.hpp"
#include <Component.hpp>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace RuamEngine
{
	class Camera : public Component
	{
		using Component::Component;

		static Camera* s_mainCamera;

		const glm::vec3 m_up = glm::vec3(0.0, 1.0, 0.0);

	public:
		Camera(const nlohmann::json& j, const unsigned int obj_id);

		float m_near_plane = 0.1f;
		float m_far_plane = 3000.0f;
		float m_fov = 45.0f;
		float m_aspect_ratio = 800.0f/600.0f;

		glm::mat4 GetProjectionMatrix();
		glm::mat4 GetViewMatrix();

		void SetAsMainCamera();
		static void EmptyMainCamera();
		static Camera* GetMainCamera();

		void update() { Component::update(); };
		void start() { SetAsMainCamera();};

		IMPL_SERIALIZE(Camera,
				 SER_FIELD(m_near_plane),
				 SER_FIELD(m_far_plane),
				 SER_FIELD(m_fov),
				 SER_FIELD(m_aspect_ratio));
	};
}
