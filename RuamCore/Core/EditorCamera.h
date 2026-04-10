#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace RuamEngine
{
	class EditorCamera
	{
		static const glm::vec3 s_up;
		static float s_nearPlane;
		static float s_farPlane;
		static float s_fov;
		static float s_aspectRatio;

		static float s_speed;
		static float s_rotationSpeed;

		static glm::vec3 s_pos;
		static glm::vec3 s_rot;

		static void UpdateCameraTransform();
	public:
	    static inline glm::vec3 Position() {return s_pos;}
		static inline glm::vec3 Rotation() {return s_rot;}
		static glm::mat4 ProjectionMatrix();
		static glm::mat4 ViewMatrix();

		friend class Engine;
	};
}
