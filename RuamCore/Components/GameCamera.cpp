#include "GameCamera.h"
#include "RuamEngine.h"

namespace RuamEngine
{
    GameCamera* GameCamera::s_mainCamera = nullptr;

	glm::mat4 GameCamera::projectionMatrix() const
	{
		return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
	}
	glm::mat4 GameCamera::viewMatrix() const
	{
		glm::vec3 pos = entity()->transform().position();
		glm::vec3 eulerRadians = glm::radians(entity()->transform().rotation());
		glm::vec3 direction;

		float pitch = eulerRadians.x;
		float yaw = eulerRadians.y;

		direction.x = cos(pitch) * sin(yaw);
		direction.y = sin(pitch);
		direction.z = cos(pitch) * cos(yaw);

		direction = glm::normalize(direction);
		return glm::lookAt(pos, pos + direction, m_up);
	};

	float GameCamera::aspectRatio() const
	{
	    return m_aspectRatio;
	}

	void GameCamera::setAspectRatio(float newAspectRatio)
	{
	    m_aspectRatio = newAspectRatio;
	}

	void GameCamera::setAsMainCamera()
	{
		s_mainCamera = this;
	}

	void GameCamera::EmptyMainCamera()
	{
		s_mainCamera = nullptr;
	}

	GameCamera* GameCamera::GetMainCamera()
	{
		return s_mainCamera;
	}

	void GameCamera::renderStart()
	{
	    setAsMainCamera();
	}

	DEF_REGISTER_COMPONENT(GameCamera);
}
