#pragma once

#include "Entity.h"
#include "../../RuamCore/Input/Input.h"
#include "RuamTime.h"

#include "glm/glm.hpp"
#include "ModelRenderer.h"

namespace RuamEngine
{

    class Shooter : public Component {

    	using Component::Component;
    	void start();


    	void update();


    public:
    	void take_damage(float damage);
    	~Shooter();
    	static Shooter* s_instance;
    	float m_bulletSpeed = 0.0f;
    	float m_bulletRadius = 1.0f;
    	float m_shootingInterval = 0.0f;
    	float m_timeSinceLastShot = 0.0f;
    	std::string m_bulletMeshPath;
    	float m_health = 100;
    	float m_damage = 3;
    };
}
