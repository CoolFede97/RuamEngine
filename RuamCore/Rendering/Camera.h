#pragma once

#include "Frustum.h"
#include "AABB.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
namespace RuamEngine
{
    struct CameraTransform
    {
        glm::vec3 pos = {0, 0, 0};
        glm::vec3 rot = {0, 0, 0};
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
	    Frustum s_frustum;

	public:
	    virtual inline glm::vec3 position() const {return m_transform.pos;}
		virtual inline glm::vec3 rotation() const {return m_transform.rot;}
		virtual inline CameraTransform cameraTransform() const {return m_transform; }
		virtual void setTransform(CameraTransform newTransform);
		virtual inline float nearPlane() { return m_nearPlane; }
		virtual inline float farPlane() { return m_farPlane; }
		virtual inline float fov() { return m_fov; }
		virtual glm::mat4 projectionMatrix() const;
		virtual glm::mat4 viewMatrix() const;
		virtual float aspectRatio() const;
		virtual bool canSeeSphere(glm::vec3 center, float radius);
		virtual bool canSeeBox(AABB aabb);
		virtual void setAspectRatio(float newAspectRatio);
		virtual glm::vec3 front() const;
		virtual glm::vec3 back() const;
		virtual glm::vec3 up() const;
		virtual glm::vec3 down() const;
		virtual glm::vec3 left() const;
		virtual glm::vec3 right() const;
	};
}
