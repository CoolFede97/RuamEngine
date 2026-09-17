#include "PhysicsManager.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "RuamTime.h"
#include "Transform.h"
namespace RuamEngine
{
    std::vector<Rigidbody*> PhysicsManager::s_rigidbodies = {};
    std::vector<BoxCollider*> PhysicsManager::s_boxColliders = {};

    void PhysicsManager::Update()
    {
        for (Rigidbody* rb : s_rigidbodies)
        {
            if (!rb->m_isKinematic && rb->m_useGravity) rb->m_velocity += rb->m_gravity*RuamTime::DeltaTime();
            rb->transform()->translate(rb->m_velocity*RuamTime::DeltaTime());
        }
        std::vector<Collision> collisions = {};

        for (int i = 0; i < s_rigidbodies.size(); i++)
        {
            for (int j = i + 1; j < s_rigidbodies.size(); j++)
            {
                glm::vec3 normal = glm::vec3(0,0,0);
                float penetration = 0;

                BoxCollider* boxA = s_rigidbodies[i]->GetBoxCollider();
                BoxCollider* boxB = s_rigidbodies[j]->GetBoxCollider();
                if (!boxA || !boxB) continue;

                if (CheckAABBOverlap(boxA, boxB, normal, penetration))
                {
                    collisions.push_back({s_rigidbodies[i], s_rigidbodies[j], normal, penetration});
                }
            }
        }

        for (Collision& col : collisions) ResolveCollision(col);
    }

    void PhysicsManager::AddRigidbody(Rigidbody* rb)
    {
        s_rigidbodies.push_back(rb);
    }
    void PhysicsManager::AddBoxCollider(BoxCollider* box)
    {
        s_boxColliders.push_back(box);
    }
    void PhysicsManager::RemoveRigidbody(Rigidbody* rb)
    {
        auto it = std::find(s_rigidbodies.begin(), s_rigidbodies.end(), rb);
        if (it != s_rigidbodies.end()) s_rigidbodies.erase(it);
    }
    void PhysicsManager::RemoveBoxCollider(BoxCollider* box)
    {
        auto it = std::find(s_boxColliders.begin(), s_boxColliders.end(), box);
        if (it != s_boxColliders.end()) s_boxColliders.erase(it);
    }
}
