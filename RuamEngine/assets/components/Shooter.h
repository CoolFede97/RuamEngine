#pragma once

#include "Object.hpp"
#include "../../RuamCore/Input/Input.h"
#include "RuamTime.h"

#include "glm/glm.hpp"
#include "MeshRenderer.h"

class Boss : public Component {
	IMPL_SIMPLE_SERIALIZE(Boss)

	using Component::Component;

	void start() {
		if (s_instance == nullptr) {
			s_instance = this;
		} else if (s_instance != this) {
			object()->removeComponent<Boss>(); // ESTO EST� MAL
		}
	}

	void update()
	{
		/*if (m_timeSinceLastShot > m_shootingInterval)
		{
			m_timeSinceLastShot = 0;
			Object* object = SceneManager::ActiveScene()->newObject(0);

			Bullet* bullet = object->addComponent<Bullet>();
			bullet->speed = 10.0f;
			bullet->direction = glm::normalize(Shooter::s_instance->object()->transform().position() - object()->transform().position());
			bullet->radius = 1.0f;
		}
		m_timeSinceLastShot += Time::DeltaTime();*/
	}
public:
	static Boss* s_instance;
protected:
	float m_shootingSpeed = 0;
	float m_shootingInterval = 0;
	float m_timeSinceLastShot = 0;
};
REGISTER_COMPONENT(Boss);

class Bullet : public Component {
	IMPL_SIMPLE_SERIALIZE(Bullet)

	using Component::Component;

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

class Shooter : public Component {
	IMPL_SIMPLE_SERIALIZE(Shooter)

	using Component::Component;

	void start()
	{
		if (s_instance == nullptr) {
			s_instance = this;
		}
		else if (s_instance != this) {
			object()->removeComponent<Shooter>(); // ESTO EST� MAL
		}
	}

	void update() {
		if (Input::GetKeyDown(KeyCode::SpaceBar_Key)) {
			Object* object = SceneManager::ActiveScene()->newObject(0);

			Bullet* bullet = object->addComponent<Bullet>();

			bullet->object()->transform().position() = s_instance->object()->transform().position();

			bullet->m_speed = m_bulletSpeed;
			bullet->m_direction = glm::normalize(Boss::s_instance->object()->transform().position() - s_instance->object()->transform().position());
			bullet->m_radius = m_bulletRadius;

			bullet->object()->addComponent<MeshRenderer>()->SetModel(m_bulletMeshPath);
		}
	}

public:
	Shooter* s_instance = nullptr;
	float m_bulletSpeed = 0.0f;
	float m_bulletRadius = 1.0f;
	std::string m_bulletMeshPath;
};

REGISTER_COMPONENT(Shooter)
