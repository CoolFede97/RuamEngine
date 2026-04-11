#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"

namespace RuamEngine
{
	class EditorCamera : public Camera
	{
		float m_speed = 50.0f;
		float m_rotationSpeed = 0.1f;

	public:
		void updateCameraTransform();
		friend class Engine;
	};
}
