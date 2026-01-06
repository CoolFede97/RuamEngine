#pragma once

#include "Entity.hpp"
#include "../../RuamCore/Input/Input.h"
#include "RuamTime.h"

#include "glm/glm.hpp"
#include "ModelRenderer.h"

class Shooter : public Component {

	using Component::Component;
	void start();


	void update();


public:
	void take_damage(float damage);
	Shooter(const nlohmann::json& j, unsigned int obj_id) : Component(obj_id)
	{
        m_bulletSpeed = j["m_bulletSpeed"];
        m_bulletRadius = j["m_bulletRadius"];
        m_shootingInterval = j["m_shootingInterval"];
        m_bulletMeshPath = j["m_bulletMeshPath"];
        m_health = j["m_health"];
        m_damage = j["m_damage"];
	}
	~Shooter()
	{
		if (s_instance == this)
			s_instance = nullptr;
	}
	static Shooter* s_instance;
	float m_bulletSpeed = 0.0f;
	float m_bulletRadius = 1.0f;
	float m_shootingInterval = 0.0f;
	float m_timeSinceLastShot = 0.0f;
	std::string m_bulletMeshPath;
	float m_health = 100;
	float m_damage = 3;
	IMPL_SERIALIZE(Shooter,
    SER_FIELD(m_bulletSpeed),
    SER_FIELD(m_bulletRadius),
    SER_FIELD(m_shootingInterval),
    SER_FIELD(m_bulletMeshPath),
    SER_FIELD(m_health),
    SER_FIELD(m_damage))
};

REGISTER_COMPONENT(Shooter)
