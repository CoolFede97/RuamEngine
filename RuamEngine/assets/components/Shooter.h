#pragma once

#include "Object.hpp"
#include "../../RuamCore/Input/Input.h"
#include "RuamTime.h"

class Boss : Component {
	IMPL_SIMPLE_SERIALIZE(Boss)
	
	void start() {
		if (s_instance == nullptr) {
			s_instance = this;
		} else if (s_instance != this) {
			object()->removeComponent<Boss>(); // ESTO ESTÁ MAL
		}
	}
	
	void update() 
	{
		if (m_timeSinceLastShot > m_shootingInterval)
		{
			m_timeSinceLastShot = 0;
			Object* object = SceneManager::ActiveScene()->newObject(0);

			Bullet* bullet = object->addComponent<Bullet>();
			bullet->speed = 10.0f;
			bullet->direction = glm::normalize(Shooter::s_instance->object()->transform().position() - object()->transform().position());
			bullet->radius = 1.0f;
		} 
		m_timeSinceLastShot += Time::deltaTime();
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

	void start()
	{
	}
	
	void update() {

	}
	 
public:
	float radius;
	float speed;
	glm::vec3 direction;
private:
};

REGISTER_COMPONENT(Bullet)

class Shooter : public Component {
	IMPL_SIMPLE_SERIALIZE(Shooter)

	void start() 
	{
		if (s_instance == nullptr) {
			s_instance = this;
		}
		else if (s_instance != this) {
			object()->removeComponent<Shooter>(); // ESTO ESTÁ MAL
		}
	}
	
	void update() {
		if (Input::GetKeyDown(KeyCode::SpaceBar_Key)) {
			Object* object = SceneManager::ActiveScene()->newObject(0);

			Bullet* bullet = object->addComponent<Bullet>();
			bullet->speed = 10.0f;
			bullet->direction = glm::normalize(Boss::s_instance->object()->transform().position() - object()->transform().position());
			bullet->radius = 1.0f;
		}
	}

public:
	Shooter* s_instance = nullptr;

};

REGISTER_COMPONENT(Shooter)
