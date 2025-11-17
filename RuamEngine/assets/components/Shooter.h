#pragma once

#include "Object.hpp"
#include "../../RuamCore/Input/Input.h"
#include "RuamTime.h"

#include "glm/glm.hpp"
#include "MeshRenderer.h"

class Shooter : public Component {
	IMPL_SIMPLE_SERIALIZE(Shooter)

	using Component::Component;
	Shooter(const nlohmann::json& j, unsigned int obj_id) : Component(obj_id) {}
	void start();


	void update();

	void take_damage(float damage);

public:
	static Shooter* s_instance;
	float m_bulletSpeed = 0.0f;
	float m_bulletRadius = 1.0f;
	float m_shootingInterval = 0.0f;
	float m_timeSinceLastShot = 0.0f;
	std::string m_bulletMeshPath;
	float m_health = 100;
	float m_damage = 3;
};

REGISTER_COMPONENT(Shooter)
