#pragma once

#include "Component.h"
#include "Entity.h"
#include "../../RuamCore/Input/Input.h"
#include "RuamTime.h"

#include "glm/glm.hpp"

#include "Bullet.h"
#include "Shooter.h"

namespace RuamEngine
{
    #define BOSS_SERIALIZED_MEMBERS(X, ...) \
        X(m_bulletSpeed, float, 0.0f, nullptr)__VA_ARGS__ \
        X(m_bulletRadius, float, 1.0f, nullptr)__VA_ARGS__ \
        X(m_shootingInterval, float, 0.0f, nullptr)__VA_ARGS__ \
        X(m_bulletMeshPath, std::string, "", nullptr)__VA_ARGS__ \
        X(m_health, float, 100.0f, nullptr)__VA_ARGS__ \
        X(m_damage, float, 10.0f, nullptr)

    class Boss : public Component
    {
        using Component::Component;

    protected:
        float m_timeSinceLastShot = 0.0f;

    public:
        BOSS_SERIALIZED_MEMBERS(DECL_MEMBER)
        std::string name() override { return "Boss"; }
        IMPL_DRAW_SERIALIZED_MEMBERS(BOSS_SERIALIZED_MEMBERS(CALL_INSPECTOR_DRAWER))
        IMPL_SERIALIZE(Boss, BOSS_SERIALIZED_MEMBERS(SER_FIELD, ,))

        void start();
        void update();
        ~Boss();

        void take_damage(float damage)
        {
            s_instance->m_health -= damage;
            if (s_instance->m_health <= 0)
            {
                s_instance->playerTransform = nullptr;
                s_instance = nullptr;
                entity()->destroy();
                SceneManager::EnqueueSceneChange("EndScene");
            }
        }

        static Boss* s_instance;
        Transform* playerTransform;
    };
}
