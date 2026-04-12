#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace RuamEngine
{
    struct CameraTransform
    {
        glm::vec3 pos;
        glm::vec3 rot;
    };

	class Camera
	{
	protected:
		const glm::vec3 m_up = glm::vec3(0.0, 1.0, 0.0);
		float m_nearPlane = 0.1f;
		float m_farPlane = 3000.0f;
		float m_fov = 45.0f;
		float m_aspectRatio = 800.0f/600.0f;

		CameraTransform m_transform;

	public:
	    virtual inline glm::vec3 position() {return m_transform.pos;}
		virtual inline glm::vec3 rotation() {return m_transform.rot;}
		virtual inline CameraTransform transform() {return m_transform; }
		virtual void setTransform(CameraTransform newTransform);
		virtual glm::mat4 projectionMatrix();
		virtual glm::mat4 viewMatrix();
		virtual void setAspectRatio(float newAspectRatio);
	};
}
