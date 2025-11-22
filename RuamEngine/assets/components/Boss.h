#pragma once

#include "Object.hpp"
#include "../../RuamCore/Input/Input.h"
#include "RuamTime.h"

#include "glm/glm.hpp"
#include "MeshRenderer.h"

#include "Bullet.h"
#include "Shooter.h"

class Boss : public Component {
	IMPL_SIMPLE_SERIALIZE(Boss)

	using Component::Component;
	Boss(const nlohmann::json& j, unsigned int obj_id) : Component(obj_id) {}

	void start() {
		if (s_instance == nullptr) {
			s_instance = this;
		} else if (s_instance != this) {
			object()->removeComponent<Boss>(); // ESTO EST� MAL
		}
	}

	void update()
	{
		if (m_timeSinceLastShot > m_shootingInterval)
		{
			m_timeSinceLastShot = 0;
			Object* object = SceneManager::ActiveScene()->newObject(0);

			object->transform().position() = s_instance->object()->transform().position();

			Bullet* bullet = &object->addComponent<Bullet>([this]() {Shooter::s_instance->take_damage(m_damage);});
			bullet->m_speed = m_bulletSpeed;
			bullet->m_direction = glm::normalize(playerTransform->position() - this->object()->transform().position());
			bullet->m_target = playerTransform;
			bullet->m_radius = m_bulletRadius;

			object->addComponent<MeshRenderer>()->SetModel(m_bulletMeshPath);
		}
		m_timeSinceLastShot += Time::DeltaTime();
	}

	void take_damage(float damage) {
		s_instance->m_health -= damage;
		if (s_instance->m_health <= 0) {
			s_instance->playerTransform = nullptr;
			s_instance = nullptr;
			object()->destroy();
			SceneManager::SetActiveScene(1);
		}
	}

public:
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

REGISTER_COMPONENT(Boss);
