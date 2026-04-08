#include "CameraController.h"

#include "Entity.h"
#include "RuamTime.h"
#include "Transform.h"
#include "../../RuamCore/Input/Input.h"
#include "../../RuamCore/Input/KeyCode.h"

#include <algorithm>

namespace RuamEngine
{
  		void CameraController::update()
		{
			// if (Input::GetKeyDown(KeyCode::LeftShift_Key)) m_speed = 25.0f;
			// else m_speed = 10.0f;

			m_mouseRotation = Vec2(0.0f, 0.0f);

			if (Input::GetKey(KeyCode::Down_Arrow)) m_mouseRotation.x -= 1;
			if (Input::GetKey(KeyCode::Up_Arrow)) m_mouseRotation.x += 1;
			if (Input::GetKey(KeyCode::Left_Arrow)) m_mouseRotation.y += 1;
			if (Input::GetKey(KeyCode::Right_Arrow)) m_mouseRotation.y -= 1;


			entity()->transform().rotation() += static_cast<glm::vec3>(m_mouseRotation * RuamTime::DeltaTime() * m_rotationSpeed);
			entity()->transform().rotation().x = std::clamp(entity()->transform().rotation().x, -89.0f, 89.0f);

			m_horizontalDirection = {0, 0, 0};
			if (Input::GetKey(KeyCode::W_Key)) m_horizontalDirection.z += 1;
			if (Input::GetKey(KeyCode::S_Key)) m_horizontalDirection.z -= 1;
			if (Input::GetKey(KeyCode::A_Key)) m_horizontalDirection.x -= 1;
			if (Input::GetKey(KeyCode::D_Key)) m_horizontalDirection.x += 1;

			m_direction = Vec3::GetDirectionFromEuler(entity()->transform().rotation());

			Vec3 left = Vec3::Up().CrossProduct(m_direction).Normalized();
			entity()->transform().position() += static_cast<glm::vec3>(m_direction * m_horizontalDirection.z * RuamTime::DeltaTime() * m_speed);
			entity()->transform().position() += static_cast<glm::vec3>(left * -m_horizontalDirection.x * RuamTime::DeltaTime() * m_speed);

		};
    // REGISTER_COMPONENT(CameraController);
}
