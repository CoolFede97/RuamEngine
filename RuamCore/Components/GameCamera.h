#pragma once

#include "Component.h"

namespace RuamEngine
{
    class GameCamera : public Component, public Camera
    {
		using Component::Component;

	    static GameCamera* s_mainCamera;

	public:
	    GameCamera(nlohmann::json transformData, const unsigned int entityId);
		~GameCamera();
		void setAsMainCamera();
		static void EmptyMainCamera();
		static GameCamera* GetMainCamera();

		void renderStart() override;
		void renderUpdate() override;
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
