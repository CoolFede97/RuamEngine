#pragma once

#include "Component.h"

namespace RuamEngine
{
    class GameCamera : public Component
    {
		using Component::Component;

	    static GameCamera* s_mainCamera;

		const glm::vec3 m_up = glm::vec3(0.0, 1.0, 0.0);
		float m_nearPlane = 0.1f;
		float m_farPlane = 3000.0f;
		float m_fov = 45.0f;
		float m_aspectRatio = 800.0f/600.0f;

	public:

	    GameCamera(nlohmann::json transformData, const unsigned int entityId);
		~GameCamera();
		glm::mat4 projectionMatrix() const;
		glm::mat4 viewMatrix() const;
		float aspectRatio() const;

		void setAspectRatio(float newAspectRatio);
		void setAsMainCamera();
		static void EmptyMainCamera();
		static GameCamera* GetMainCamera();

		void renderStart() override;

		std::vector<FieldInfo> fields() override
		{
		    return
    		{
                makeFieldInfo<float>("m_nearPlane", m_nearPlane),
                makeFieldInfo<float>("m_farPlane", m_farPlane),
                makeFieldInfo<float>("m_fov", m_fov),
                makeFieldInfo<float>("m_aspectRatio", m_aspectRatio)
    		};
		}

	private:
		DECL_REGISTER_COMPONENT(GameCamera)
	};
}
