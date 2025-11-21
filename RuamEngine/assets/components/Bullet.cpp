#include "Bullet.h"
#include "Boss.h"
void Bullet::update()
{
    if (m_isPlayerBullet && Boss::s_instance == nullptr) return;
		object()->transform().setPosition(object()->transform().position() + m_direction * m_speed * Time::DeltaTime());
		if (glm::length(object()->transform().position() - m_target->position()) <= m_radius)
		{
			std::cout << "Bullet hit target!\n";
			m_callback();
			object()->destroy();
		}
		if (object()->transform().position().length() > 800.0f) // Arbitrary large distance to destroy bullet
		{
			object()->destroy();
		}
}
