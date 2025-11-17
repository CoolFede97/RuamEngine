#pragma once

#include "Object.hpp"
#include "../../RuamCore/Input/Input.h"
#include "RuamTime.h"

#include "glm/glm.hpp"
#include "MeshRenderer.h"
#include <functional>

class Bullet : public Component {
	IMPL_SIMPLE_SERIALIZE(Bullet)

	using Component::Component;

	Bullet(unsigned int obj_id, std::function<void()> callback)
		: Component(obj_id), m_callback(callback) {

	}

	Bullet(const nlohmann::json& j, unsigned int obj_id) : Component(obj_id) {}

	void start()
	{
	}

	void update()
	{
		object()->transform().setPosition(object()->transform().position() + m_direction * m_speed * Time::DeltaTime());
		if (glm::length(object()->transform().position() - m_target->position()) <= m_radius)
		{
			std::cout << "Bullet hit target!\n";
			m_callback();
			object()->destroy();
		}
		if (object()->transform().position().length() > 800.0f) // Arbitrary large distance to destroy bullet
		{
			object()->destroy();
		}
	}

public:
	float m_radius;
	float m_speed;
	glm::vec3 m_direction;
	Transform* m_target;
private:
	std::function<void()> m_callback;
};


REGISTER_COMPONENT(Bullet)
