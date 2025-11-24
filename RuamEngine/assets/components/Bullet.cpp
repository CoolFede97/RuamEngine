#include "Bullet.h"
#include "Boss.h"
#include "Shooter.h"
void Bullet::update()
{
    if (m_isPlayerBullet && Boss::s_instance == nullptr) return;
		object()->transform().setPosition(object()->transform().position() + m_direction * m_speed * Time::DeltaTime());

		if (glm::length(object()->transform().position() - m_target) <= m_radius)
		{
			std::cout << "Bullet hit target!\n";
			m_callback();
			if (m_isPlayerBullet && Boss::s_instance!= nullptr) object()->destroy();
			// MOSTRARLE A TOMI POR QUE ESTO ES PELIGROSO!!!
		}
		if (object()->transform().position().length() > 800.0f) // Arbitrary large distance to destroy bullet
		{
			object()->destroy();
		}
}
