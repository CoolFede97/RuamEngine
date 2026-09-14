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
        glm::vec3 m_velocity;
        float m_mass;
        bool m_useGravity;
        bool m_isKinematic;
        glm::vec3 m_gravity;
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
