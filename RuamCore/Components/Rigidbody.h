#pragma once

#include "BoxCollider.h"
#include "Component.h"
#include "Vec3.h"
#include "nlohmann/json.hpp"
#include "JsonConverters.h"
namespace RuamEngine
{
    class Rigidbody : public Component
    {

    public:
        glm::vec3 m_velocity = {0,0,0};
        float m_mass = 1;
        bool m_useGravity = true;
        bool m_isKinematic = false;
        glm::vec3 m_gravity = {0, -9.8f, 0};
        std::vector<FieldInfo> fields() override
		{
		    return
      		{
                makeFieldInfo<glm::vec3>("m_velocity", m_velocity),
                makeFieldInfo<float>("m_mass", m_mass),
                makeFieldInfo<bool>("m_useGravity", m_useGravity),
                makeFieldInfo<bool>("m_isKinematic", m_isKinematic),
                makeFieldInfo<glm::vec3>("m_gravity", m_gravity)
      		};
		}
        Rigidbody(unsigned int entityId);
        ~Rigidbody();

        BoxCollider* GetBoxCollider();

        DECL_REGISTER_COMPONENT(Rigidbody)
    };
}
