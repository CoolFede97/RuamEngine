#include "PhysicsManager.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "RuamTime.h"
#include "Transform.h"
#include "Entity.h"

namespace RuamEngine
{
    std::vector<BoxCollider*> PhysicsManager::s_boxColliders = {};
    std::map<colliderPairKey, Collision> PhysicsManager::s_previousCollisions = {};

    void PhysicsManager::Update()
    {
        std::set<unsigned int> rbIdsUpdated = {};
        for (Collider* col : s_boxColliders)
        {
            Rigidbody* rb = col->getComponent<Rigidbody>();
            if (!col->enabled() || !rb || rbIdsUpdated.count(rb->id())) continue;
            if (rb->m_useGravity) rb->m_velocity += rb->m_gravity*RuamTime::DeltaTime();
            rb->transform()->translate(rb->m_velocity*RuamTime::DeltaTime());
            rbIdsUpdated.insert(rb->id());
        }
        std::vector<Collision> collisions = {};

        for (int i = 0; i < s_boxColliders.size(); i++)
        {
            for (int j = i + 1; j < s_boxColliders.size(); j++)
            {
                glm::vec3 normal = glm::vec3(0,0,0);
                float penetration = 0;

                BoxCollider* boxA = s_boxColliders[i];
                BoxCollider* boxB = s_boxColliders[j];
                if (!boxA || !boxB) continue;

                if (CheckAABBOverlap(boxA, boxB, normal, penetration))
                {
                    collisions.push_back({s_boxColliders[i], s_boxColliders[j], normal, penetration});
                }
            }
        }

        for (Collision& col : collisions) ResolveCollision(col);

        std::map<colliderPairKey, Collision> currentCollisions;
        for (Collision& col : collisions)
        {
            colliderPairKey pair = {col.a, col.b};
            currentCollisions[pair] = col;
            CollisionInfo colInfoA {col.b, col.normal, col.penetration};
            CollisionInfo colInfoB {col.a, -col.normal, col.penetration};
            if (!s_previousCollisions.count(pair)) { pair.first->entity()->onCollisionEnter(colInfoA); pair.second->entity()->onCollisionEnter(colInfoB);}
            else { pair.first->entity()->onCollisionStay(colInfoA); pair.second->entity()->onCollisionStay(colInfoB);}
        }
        for (auto& [key, oldCollision] : s_previousCollisions)
        {
            CollisionInfo colInfoA { oldCollision.b, oldCollision.normal, oldCollision.penetration };
            CollisionInfo colInfoB { oldCollision.a, -oldCollision.normal, oldCollision.penetration };
            if (!currentCollisions.count(key)) { key.first->entity()->onCollisionExit(colInfoA); key.second->entity()->onCollisionExit(colInfoB); }
        }
        s_previousCollisions = currentCollisions;
    }

    void PhysicsManager::AddBoxCollider(BoxCollider* box)
    {
        s_boxColliders.push_back(box);
    }
    void PhysicsManager::RemoveBoxCollider(BoxCollider* box)
    {
        auto it = std::find(s_boxColliders.begin(), s_boxColliders.end(), box);
        if (it != s_boxColliders.end()) s_boxColliders.erase(it);
    }
}
