#pragma once

#include "Component.hpp"
#include "Object.hpp"
#include "Renderer.h"
#include "Vertex.h"
#include "SSBO.h"
#include "RuamTime.h"
#include "../../RuamCore/Input/Input.h"
#include "../../RuamCore/Input/KeyCode.h"
#include "../../RuamCore/Input/Cursor.h"
#include <algorithm>
using namespace RuamEngine;

class CameraController : public Component
{
	using Component::Component;

	IMPL_SIMPLE_SERIALIZE(CameraController)
	Vec3 m_direction;
	Vec3 m_horizontalDirection;
	Vec2 m_mouseRotation;
public:
	CameraController(const nlohmann::json& j, const unsigned int obj_id) : Component(obj_id) {};
	float m_speed = 0;
	float m_rotationSpeed = 0;
private:
	void update()
	{
		if (Input::GetKeyDown(KeyCode::LeftShift_Key)) m_speed = 25.0f;
		else m_speed = 10.0f;

		m_mouseRotation = Vec2(0.0f, 0.0f);

		if (Input::GetKeyDown(KeyCode::Down_Arrow)) m_mouseRotation.x -= 1;
		if (Input::GetKeyDown(KeyCode::Up_Arrow)) m_mouseRotation.x += 1;
		if (Input::GetKeyDown(KeyCode::Left_Arrow)) m_mouseRotation.y += 1;
		if (Input::GetKeyDown(KeyCode::Right_Arrow)) m_mouseRotation.y -= 1;


		object()->transform().rotation() += static_cast<glm::vec3>(m_mouseRotation * Time::DeltaTime() * m_rotationSpeed);
		//object()->transform().rotation() += static_cast<glm::vec3>(Input::GetMouseDeltaNorm() * Time::DeltaTime() * m_rotationSpeed);
		object()->transform().rotation().x = std::clamp(object()->transform().rotation().x, -89.0f, 89.0f);
		//std::cout << "Rotation variation: " << static_cast<glm::vec3>(Input::GetMouseDeltaNorm() * Time::DeltaTime() * m_rotationSpeed) << "\n";
		//std::cout << "Rotation: " << object()->transform().rotation() << "\n";
		//std::cout << "Normal variation: " << Input::GetMouseDeltaNorm() << "\n";

		m_horizontalDirection = {0, 0, 0};
		if (Input::GetKeyDown(KeyCode::W_Key)) m_horizontalDirection.z += 1;
		if (Input::GetKeyDown(KeyCode::S_Key)) m_horizontalDirection.z -= 1;
		if (Input::GetKeyDown(KeyCode::A_Key)) m_horizontalDirection.x -= 1;
		if (Input::GetKeyDown(KeyCode::D_Key)) m_horizontalDirection.x += 1;

		m_direction = Vec3::GetDirectionFromEuler(object()->transform().rotation());
		//std::cout << "Direction: " << m_direction << "\n";

		Vec3 left = Vec3::Up().CrossProduct(m_direction).Normalized();
		object()->transform().position() += static_cast<glm::vec3>(m_direction * m_horizontalDirection.z * Time::DeltaTime() * m_speed);
		object()->transform().position() += static_cast<glm::vec3>(left * -m_horizontalDirection.x * Time::DeltaTime() * m_speed);

	};
	void start() {};
};

REGISTER_COMPONENT(CameraController)
