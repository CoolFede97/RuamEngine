#include "PhysicsManager.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "RuamTime.h"
#include "Transform.h"
#include "Entity.h"

namespace RuamEngine
{
    std::vector<Rigidbody*> PhysicsManager::s_rigidbodies = {};
    std::vector<BoxCollider*> PhysicsManager::s_boxColliders = {};
    std::map<rbPairKey, Collision> PhysicsManager::s_previousCollisions = {};

    void PhysicsManager::Update()
    {
        for (Rigidbody* rb : s_rigidbodies)
        {
            if (!rb->enabled()) return;
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

        std::map<rbPairKey, Collision> currentCollisions;
        for (Collision& col : collisions)
        {
            rbPairKey pair = {col.a, col.b};
            currentCollisions[pair] = col;
            CollisionInfo colInfoA {col.b->entity(), col.normal, col.penetration};
            CollisionInfo colInfoB {col.a->entity(), -col.normal, col.penetration};
            if (!s_previousCollisions.count(pair)) { pair.first->entity()->onCollisionEnter(colInfoA); pair.second->entity()->onCollisionEnter(colInfoB);}
            else { pair.first->entity()->onCollisionStay(colInfoA); pair.second->entity()->onCollisionStay(colInfoB);}
        }
        for (auto& [key, oldCollision] : s_previousCollisions)
        {
            CollisionInfo colInfoA { oldCollision.b->entity(), oldCollision.normal, oldCollision.penetration };
            CollisionInfo colInfoB { oldCollision.a->entity(), -oldCollision.normal, oldCollision.penetration };
            if (!currentCollisions.count(key)) { key.first->entity()->onCollisionExit(colInfoA); key.second->entity()->onCollisionExit(colInfoB); }
        }
        s_previousCollisions = currentCollisions;
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
