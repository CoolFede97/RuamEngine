#include "Shooter.h"

#include "Boss.h"
#include "Bullet.h"
#include "ModelRenderer.h"

namespace RuamEngine
{
    Shooter* Shooter::s_instance = nullptr;
    Shooter::~Shooter()
    {
    	if (s_instance == this)
    		s_instance = nullptr;
    }

    void Shooter::start()
    {
        std::cout << "Start se llama después de cambiar de escena!\n";
        if (s_instance == nullptr)
        {
    		s_instance = this;
    	}
    	else if (s_instance != this)
    	{
    		entity()->removeComponent<Shooter>();
    	}
        if (Boss::s_instance != nullptr)
    	Boss::s_instance->playerTransform = &entity()->transform();
    	else std::cout << "Error: Boss instance does not exist\n";
    }

    void Shooter::update()
    {
    	if (Input::GetKeyDown(KeyCode::SpaceBar_Key) && m_timeSinceLastShot > m_shootingInterval)
    	{
    	    if (Boss::s_instance == nullptr) return;
    		m_timeSinceLastShot = 0.0f;
    		Entity* entity = SceneManager::ActiveScene()->createEntity(0);

    		Bullet& bullet = entity->addComponent<Bullet>([this]() {Boss::s_instance->take_damage(m_damage);});

    		bullet.entity()->transform().position() = s_instance->entity()->transform().position();

    		bullet.m_speed = m_bulletSpeed;
    		bullet.m_direction = glm::normalize(Boss::s_instance->entity()->transform().position() - s_instance->entity()->transform().position());
    		bullet.m_target = Boss::s_instance->entity()->transform().position();
    		bullet.m_radius = m_bulletRadius;

    		bullet.entity()->addComponent<ModelRenderer>()->m_shaderProgramType = ShaderProgramType::general;
    		bullet.entity()->getComponent<ModelRenderer>()->setModel(m_bulletMeshPath);
    	}
    	m_timeSinceLastShot += Time::DeltaTime();
    }

    void Shooter::take_damage(float damage)
    {
    	s_instance->m_health -= damage;
    	if (s_instance->m_health <= 0)
    	{
    	}
    }

    REGISTER_COMPONENT(Shooter);
}
