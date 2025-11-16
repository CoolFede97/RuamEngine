#pragma once

#include "Object.hpp"
#include "../../RuamCore/Input/Input.h"
#include "RuamTime.h"

#include "glm/glm.hpp"
#include "MeshRenderer.h"

class Bullet : public Component {
	IMPL_SIMPLE_SERIALIZE(Bullet)

	using Component::Component;
	Bullet(const nlohmann::json& j, unsigned int obj_id) : Component(obj_id) {}

	void start()
	{
	}

	void update()
	{
		object()->transform().setPosition(object()->transform().position() + m_direction * m_speed * Time::DeltaTime());
	}

public:
	float m_radius;
	float m_speed;
	glm::vec3 m_direction;
private:
};


REGISTER_COMPONENT(Bullet)
