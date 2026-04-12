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
    glm::mat4 Camera::projectionMatrix()
	{
		return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
	}
	glm::mat4 Camera::viewMatrix()
	{
		glm::vec3 eulerRadians = glm::radians(m_transform.rot);
		glm::vec3 direction;

		float pitch = eulerRadians.x;
		float yaw = eulerRadians.y;

		direction.x = cos(pitch) * sin(yaw);
		direction.y = sin(pitch);
		direction.z = cos(pitch) * cos(yaw);

		direction = glm::normalize(direction);
		return glm::lookAt(m_transform.pos, m_transform.pos + direction, m_up);
	};
	void Camera::setTransform(CameraTransform newTransform)
	{
	    m_transform = newTransform;
	}
}
