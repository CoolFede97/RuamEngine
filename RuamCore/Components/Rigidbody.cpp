#include "Rigidbody.h"
#include "BoxCollider.h"
#include "Component.h"
#include "PhysicsManager.h"
#include "Entity.h"
#include "JsonConverters.h"

namespace RuamEngine
{
    Rigidbody::Rigidbody(unsigned int entityId) : Component(entityId),
    m_velocity(0,0,0), m_mass(1), m_isKinematic(false), m_useGravity(true), m_gravity(0, -1, 0)
    {
        PhysicsManager::AddRigidbody(this);
    }
    Rigidbody::~Rigidbody()
    {
        PhysicsManager::RemoveRigidbody(this);
    }

    BoxCollider* Rigidbody::GetBoxCollider()
    {
        BoxCollider* box = entity()->getComponent<BoxCollider>();
        if (box) return box;
        else return nullptr;
    }
    DEF_REGISTER_COMPONENT(Rigidbody);
}
