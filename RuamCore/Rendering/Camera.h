#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace RuamEngine
{
	class Camera
	{
	protected:
		const glm::vec3 m_up = glm::vec3(0.0, 1.0, 0.0);
		float m_nearPlane = 0.1f;
		float m_farPlane = 3000.0f;
		float m_fov = 45.0f;
		float m_aspectRatio = 800.0f/600.0f;

		glm::vec3 m_pos = {0,0,0};
		glm::vec3 m_rot = {0,0,0};

	public:
	    virtual inline glm::vec3 position() {return m_pos;}
		virtual inline glm::vec3 rotation() {return m_rot;}
		virtual glm::mat4 projectionMatrix();
		virtual glm::mat4 viewMatrix();
	};
}
