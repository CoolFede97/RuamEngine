#include "Camera.h"
#include "Cursor.h"
#include "Input.h"
#include "KeyCode.h"
#include "Vec2.h"
#include "Vec3.h"
#include "RuamTime.h"

#include <algorithm>

namespace RuamEngine
{
    glm::mat4 Camera::projectionMatrix() const
	{
		return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
	}
	glm::mat4 Camera::viewMatrix() const
	{
	    return glm::lookAt(position(), position() + front(), m_up);
	};
	glm::vec3 Camera::front() const
	{
        float pitch = glm::radians(rotation().x);
        float yaw   = glm::radians(rotation().y);

        glm::vec3 front;
        front.x = cos(pitch) * sin(yaw);
        front.y = sin(pitch);
        front.z = cos(pitch) * cos(yaw);

        return glm::normalize(front);
	}
	glm::vec3 Camera::back() const
	{
	    return -front();
	}
	glm::vec3 Camera::up() const
	{
        return glm::normalize(
                glm::cross(front(), right())
            );
	}
    glm::vec3 Camera::down() const
    {
        return -up();
    }
    glm::vec3 Camera::left() const
    {
        return -right();
    }
    glm::vec3 Camera::right() const
    {
        return glm::normalize(
                glm::cross(m_up, front())
            );
    }
	float Camera::aspectRatio() const
	{
	    return m_aspectRatio;
	}
	bool Camera::canSeeSphere(glm::vec3 center, float radius) {return s_frustum.sphereInside(center, radius, this);}
    bool Camera::canSeeBox(AABB aabb) { return s_frustum.boxInside(aabb, this);}
	void Camera::setTransform(CameraTransform newTransform)
	{
	    m_transform = newTransform;
	}
	void Camera::setAspectRatio(float newAspectRatio)
	{
	    m_aspectRatio = newAspectRatio;
	}
}
