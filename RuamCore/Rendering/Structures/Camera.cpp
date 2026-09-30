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
		return glm::perspectiveLH(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
	}
	glm::mat4 Camera::viewMatrix() const
	{
	    std::cout << "Front: " << Vec3(front()) << "\n";
	    return glm::lookAtLH(position(), position() + front(), m_up);
	};
	glm::vec3 Camera::front() const
	{
        glm::vec3 front = Vec3::GetDirectionFromEuler({rotation().x, rotation().y, 0.0});
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
