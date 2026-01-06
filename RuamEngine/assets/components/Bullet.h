#pragma once

#include "Entity.hpp"
#include "../../RuamCore/Input/Input.h"
#include "RuamTime.h"

#include "glm/glm.hpp"
#include "ModelRenderer.h"
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

	void update();

public:
    bool m_isPlayerBullet = false;
	float m_radius;
	float m_speed;
	glm::vec3 m_direction;
	glm::vec3 m_target;
private:
	std::function<void()> m_callback;
};


REGISTER_COMPONENT(Bullet)
