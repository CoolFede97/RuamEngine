

namespace RuamEngine
{
	class Camera : public Component
	{
		using Component::Component;

		static CameraPtr s_mainCamera = nullptr;
		const glm::vec3 m_up = glm::vec3(0.0, 1.0, 0.0);

	public: 
		float m_near_plane = 0.1f;
		float m_far_plane = 100.0f;
		float m_fov = 45.0f; 
		float m_aspect_ratio = 800.0f/600.0f;

		glm::mat4 GetProjectionMatrix() const { return glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near_plane, m_far_plane); }
		glm::mat4 GetViewMatrix() const
		{
			glm::vec3 pos = object()->transform().position();
			glm::vec3 eulerRadians = glm::radians(object()->transform().rotation());
			glm::mat4 rotationMatrix = glm::yawPitchRoll(eulerRadians.y, eulerRadians.x, eulerRadians.z);
			glm::vec3 direction = glm::vec3(rotationMatrix * glm::vec4(0.0, 0.0, 1.0, 0.0));
			return glm::lookAt(pos, pos + direction, m_up);
		};

		void SetAsMainCamera()
		{
			s_mainCamera = shared_from_this();
		}

		static CameraPtr& GetMainCamera()
		{
			return s_mainCamera;
		}

		glm::mat4 GetViewMatrix() const
		{
			glm::vec3 pos = object()->transform().position();
			glm::vec3 eulerRadians = glm::radians(object()->transform().rotation());
			glm::mat4 rotationMatrix = glm::yawPitchRoll(eulerRadians.y, eulerRadians.x, eulerRadians.z);
			glm::vec3 direction = glm::vec3(rotationMatrix * glm::vec4(0.0, 0.0, 1.0, 0.0));
			return glm::lookAt(pos, pos + direction, m_up);
		}
	};
	using CameraPtr = std::shared_ptr<Camera>;
}