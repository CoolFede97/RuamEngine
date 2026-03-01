#include "Boss.h"
#include "ModelRenderer.h"

namespace RuamEngine
{
    Boss* Boss::s_instance = nullptr;

    void Boss::start()
    {
        if (s_instance == nullptr)
        {
            s_instance = this;
        }
        else if (s_instance != this)
        {
            entity()->removeComponent<Boss>();
        }
    }

    void Boss::update()
    {
        if (m_timeSinceLastShot > m_shootingInterval)
        {
            m_timeSinceLastShot = 0;
            Entity* entity = SceneManager::ActiveScene()->createEntity(0);

            entity->transform().position() = s_instance->entity()->transform().position();

            Bullet* bullet = entity->addComponent<Bullet>([this]() {Shooter::s_instance->take_damage(m_damage);});
            bullet->m_speed = m_bulletSpeed;
            bullet->m_direction = glm::normalize(playerTransform->position() - this->entity()->transform().position());
            bullet->m_target = playerTransform->position();
            bullet->m_radius = m_bulletRadius;

            bullet->entity()->addComponent<ModelRenderer>()->m_shaderProgramType = ShaderProgramType::general;
            bullet->entity()->getComponent<ModelRenderer>()->setModel(m_bulletMeshPath);
        }
        m_timeSinceLastShot += Time::DeltaTime();
    }

    Boss::~Boss()
    {
        if (s_instance == this)
        {
            s_instance = nullptr;
            playerTransform = nullptr;
        }
    }

    REGISTER_COMPONENT(Boss);
}
