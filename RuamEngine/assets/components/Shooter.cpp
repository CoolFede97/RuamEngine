#include "Shooter.h"

#include "Boss.h"
#include "Bullet.h"

void Shooter::start()
{
	if (s_instance == nullptr) {
		s_instance = this;
	}
	else if (s_instance != this) {
		object()->removeComponent<Shooter>(); // ESTO EST� MAL
	}
	Boss::s_instance->playerTransform = &object()->transform();
}

void Shooter::update() {
	if (Input::GetKeyDown(KeyCode::SpaceBar_Key) && m_timeSinceLastShot > m_shootingInterval)
	{
	    if (Boss::s_instance == nullptr) return;
		m_timeSinceLastShot = 0.0f;
		Object* object = SceneManager::ActiveScene()->newObject(0);

		Bullet& bullet = object->addComponent<Bullet>([this]() {Boss::s_instance->take_damage(m_damage);});

		bullet.object()->transform().position() = s_instance->object()->transform().position();

		bullet.m_speed = m_bulletSpeed;
		bullet.m_direction = glm::normalize(Boss::s_instance->object()->transform().position() - s_instance->object()->transform().position());
		bullet.m_target = &Boss::s_instance->object()->transform();
		bullet.m_radius = m_bulletRadius;

		bullet.object()->addComponent<MeshRenderer>()->SetModel(m_bulletMeshPath);
	}
	m_timeSinceLastShot += Time::DeltaTime();
}

void Shooter::take_damage(float damage) {
	s_instance->m_health -= damage;
	if (s_instance->m_health <= 0) {
	}
}

Shooter* Shooter::s_instance = nullptr;
