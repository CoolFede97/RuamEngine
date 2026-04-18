#pragma once

#include "Component.h"

namespace RuamEngine
{
    class GameCamera : public Component
    {
        #define CAMERA_SERIALIZED_MEMBERS(X, ...) \
    	X(m_nearPlane, float, 0.1f, nullptr)__VA_ARGS__	\
    	X(m_farPlane, float, 3000.0f, nullptr)__VA_ARGS__	\
    	X(m_fov, float, 45.0f, nullptr)__VA_ARGS__	\
    	X(m_aspectRatio, float, 800.0f/600.0f, nullptr)

		using Component::Component;

	    static GameCamera* s_mainCamera;

		const glm::vec3 m_up = glm::vec3(0.0, 1.0, 0.0);
		CAMERA_SERIALIZED_MEMBERS(DECL_MEMBER)

	public:

		IMPL_DRAW_SERIALIZED_MEMBERS(CAMERA_SERIALIZED_MEMBERS(CALL_INSPECTOR_DRAWER))

		glm::mat4 projectionMatrix();
		glm::mat4 viewMatrix();

		void setAspectRatio(float newAspectRatio);

		void setAsMainCamera();
		static void EmptyMainCamera();
		static GameCamera* GetMainCamera();

		void renderStart() override;

		std::string name() const override { return "GameCamera"; }
		IMPL_SERIALIZE(GameCamera, CAMERA_SERIALIZED_MEMBERS(SER_FIELD, ,))
	private:
		DECL_REGISTER_COMPONENT(GameCamera)
	};
}
