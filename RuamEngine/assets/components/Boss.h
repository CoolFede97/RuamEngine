#pragma once

#include "Entity.h"
#include "../../RuamCore/Input/Input.h"
#include "RuamTime.h"

#include "glm/glm.hpp"
#include "ModelRenderer.h"

#include "Bullet.h"
#include "Shooter.h"

class Boss : public Component {
	void start() {
        if (s_instance == nullptr)
		{
			s_instance = this;
		}
	    else if (s_instance != this)
		{
			entity()->removeComponent<Boss>(); // ESTO EST� MAL
		}
	}

	void update()
	{
		if (m_timeSinceLastShot > m_shootingInterval)
		{
			m_timeSinceLastShot = 0;
			Entity* entity = SceneManager::ActiveScene()->createEntity(0);

			entity->transform().position() = s_instance->entity()->transform().position();

			Bullet* bullet = &entity->addComponent<Bullet>([this]() {Shooter::s_instance->take_damage(m_damage);});
			bullet->m_speed = m_bulletSpeed;
			bullet->m_direction = glm::normalize(playerTransform->position() - this->entity()->transform().position());
			bullet->m_target = playerTransform->position();
			bullet->m_radius = m_bulletRadius;


			bullet->entity()->addComponent<ModelRenderer>()->m_shaderProgramType = ShaderProgramType::general;
			bullet->entity()->getComponent<ModelRenderer>()->setModel(m_bulletMeshPath);
		}
		m_timeSinceLastShot += Time::DeltaTime();
	}


public:
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
	~Boss()
	{
    	if (s_instance == this) {
    		s_instance = nullptr;
    		playerTransform = nullptr;
    	}
	}
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
