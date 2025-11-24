#include "Camera.h"
namespace RuamEngine
{
	Camera* Camera::s_mainCamera = nullptr;

	glm::mat4 Camera::GetProjectionMatrix()
	{
		return glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near_plane, m_far_plane);
	}
	glm::mat4 Camera::GetViewMatrix()
	{
		glm::vec3 pos = object()->transform().position();
		glm::vec3 eulerRadians = glm::radians(object()->transform().rotation());
		glm::vec3 direction;

		float pitch = eulerRadians.x;
		float yaw = eulerRadians.y;

		direction.x = cos(pitch) * sin(yaw);
		direction.y = sin(pitch);
		direction.z = cos(pitch) * cos(yaw);

		direction = glm::normalize(direction);
		return glm::lookAt(pos, pos + direction, m_up);
	};
	void Camera::SetAsMainCamera()
	{
		s_mainCamera = this;
	}

	void Camera::EmptyMainCamera()
	{
		s_mainCamera = nullptr;
	}

	Camera* Camera::GetMainCamera()
	{
		return s_mainCamera;
	}

	Camera::Camera(const nlohmann::json& j, const unsigned int obj_id)
	: Component(obj_id), m_near_plane(j["m_near_plane"]), m_far_plane(j["m_far_plane"]), m_fov(j["m_fov"]), m_aspect_ratio(j["m_aspect_ratio"])
	{}
REGISTER_COMPONENT(Camera)
}
