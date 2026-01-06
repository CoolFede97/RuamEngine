#pragma once

#include "Entity.hpp"
#include "Renderer.h"
#include "SceneManager.hpp"
#include "Vertex.h"
#include "SSBO.h"
#include "RuamTime.h"
#include "../../RuamCore/Input/Input.h"
#include "../../RuamCore/Input/KeyCode.h"
#include "../../RuamCore/Input/Cursor.h"
#include <algorithm>
#include "ModelRenderer.h"

using namespace RuamEngine;

class CameraController : public Component
{
	using Component::Component;

	Vec3 m_direction;
	Vec3 m_horizontalDirection;
	Vec2 m_mouseRotation;
public:
	CameraController(const nlohmann::json& j, const unsigned int obj_id) : Component(obj_id)
	{
		m_speed = j["m_speed"];
		m_rotationSpeed = j["m_rotationSpeed"];
	};
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


		entity()->transform().rotation() += static_cast<glm::vec3>(m_mouseRotation * Time::DeltaTime() * m_rotationSpeed);
		//entity()->transform().rotation() += static_cast<glm::vec3>(Input::GetMouseDeltaNorm() * Time::DeltaTime() * m_rotationSpeed);
		entity()->transform().rotation().x = std::clamp(entity()->transform().rotation().x, -89.0f, 89.0f);
		//std::cout << "Rotation variation: " << static_cast<glm::vec3>(Input::GetMouseDeltaNorm() * Time::DeltaTime() * m_rotationSpeed) << "\n";
		//std::cout << "Rotation: " << entity()->transform().rotation() << "\n";
		//std::cout << "Normal variation: " << Input::GetMouseDeltaNorm() << "\n";

		m_horizontalDirection = {0, 0, 0};
		if (Input::GetKeyDown(KeyCode::W_Key)) m_horizontalDirection.z += 1;
		if (Input::GetKeyDown(KeyCode::S_Key)) m_horizontalDirection.z -= 1;
		if (Input::GetKeyDown(KeyCode::A_Key)) m_horizontalDirection.x -= 1;
		if (Input::GetKeyDown(KeyCode::D_Key)) m_horizontalDirection.x += 1;

		m_direction = Vec3::GetDirectionFromEuler(entity()->transform().rotation());
		//std::cout << "Direction: " << m_direction << "\n";

		Vec3 left = Vec3::Up().CrossProduct(m_direction).Normalized();
		entity()->transform().position() += static_cast<glm::vec3>(m_direction * m_horizontalDirection.z * Time::DeltaTime() * m_speed);
		entity()->transform().position() += static_cast<glm::vec3>(left * -m_horizontalDirection.x * Time::DeltaTime() * m_speed);

	};
	void start()
	{
	    // for (auto& obj : SceneManager::ActiveScene()->getObjects())
     //    {
     //        if (obj->name() == "Skybox")
     //        {
     //            for (auto mesh : obj->getComponent<ModelRenderer>()->m_model->m_meshes)
     //            {
     //                mesh.m_material->m_shininess = 10000.0f;
     //            }
     //        }
     //    }
	};
	IMPL_SERIALIZE(CameraController,
	SER_FIELD(m_speed),
	SER_FIELD(m_rotationSpeed))
};

REGISTER_COMPONENT(CameraController)
