#include "Bullet.h"
#include "Boss.h"
#include "Shooter.h"

namespace RuamEngine
{
    Bullet::~Bullet(){}

    void Bullet::update()
    {
        if (m_isPlayerBullet && Boss::s_instance == nullptr) return;
    		entity()->transform().setPosition(entity()->transform().position() + m_direction * m_speed * Time::DeltaTime());

    		if (glm::length(entity()->transform().position() - m_target) <= m_radius)
    		{
    			m_callback();
    			if (m_isPlayerBullet && Boss::s_instance!= nullptr) entity()->destroy();
    			// MOSTRARLE A TOMI POR QUE ESTO ES PELIGROSO!!!
    		}
    		if (entity()->transform().position().length() > 800.0f) // Arbitrary large distance to destroy bullet
    		{
    			entity()->destroy();
    		}
    }
}
