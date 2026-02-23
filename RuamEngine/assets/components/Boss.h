#pragma once

#include "Component.h"
#include "Entity.h"
#include "../../RuamCore/Input/Input.h"
#include "RuamTime.h"

#include "glm/glm.hpp"
#include "ModelRenderer.h"

#include "Bullet.h"
#include "Shooter.h"

namespace RuamEngine
{
    class Boss : public Component {
    	void start();

    	void update();

    public:
        IMPL_SIMPLE_SERIALIZE(Boss)
        virtual inline std::string name() override { return "Boss";}
        IMPL_forEachSerializedField(;);
    	void take_damage(float damage) {
    		s_instance->m_health -= damage;
    		if (s_instance->m_health <= 0) {
    			s_instance->playerTransform = nullptr;
    			s_instance = nullptr;
    			entity()->destroy();
    			SceneManager::EnqueueSceneChange(2);
    		}
    	}
    	using Component::Component;
    	~Boss();

    	static Boss* s_instance;
        Transform* playerTransform;
    	float m_bulletSpeed = 0;
    	float m_bulletRadius = 1.0f;
    	float m_shootingInterval = 0;
    	std::string m_bulletMeshPath;
    	float m_health = 100;
    	float m_damage = 10;

    protected:
    	float m_timeSinceLastShot = 0;
    };
}
