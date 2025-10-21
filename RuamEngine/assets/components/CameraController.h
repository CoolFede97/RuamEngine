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

	Vec3 m_direction;
	Vec3 m_horizontalDirection;
	float m_speed;
	void update()
	{
		object()->transform().rotation() += Input::GetMouseDeltaNorm() * Time::DeltaTime();
		object()->transform().rotation().x = std::clamp(object()->transform().rotation().x, -89.0f, 89.0f);


		m_horizontalDirection = {0, 0, 0};
		if (Input::GetKeyDown(KeyCode::W_Key)) m_horizontalDirection.z += 1;
		if (Input::GetKeyDown(KeyCode::S_Key)) m_horizontalDirection.z -= 1;
		if (Input::GetKeyDown(KeyCode::A_Key)) m_horizontalDirection.x -= 1;
		if (Input::GetKeyDown(KeyCode::D_Key)) m_horizontalDirection.x += 1;
		
		m_direction = Vec3::GetDirectionFromEuler(object()->transform().rotation());

		object()->transform().position() += m_direction * m_horizontalDirection.z * Time::DeltaTime() * m_speed;
		object()->transform().position() += m_direction.CrossProduct(Vec3::Up() * m_horizontalDirection.x * Time::DeltaTime() * m_speed);

		Camera::GetMainCamera()->object()->transform().position() += glm::vec3(0, 0, Time::DeltaTime() * 100);
	};
	void start() {};
};
