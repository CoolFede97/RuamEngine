#pragma once

#include "Collision.h"
#include "Rigidbody.h"
#include "BoxCollider.h"

#include <vector>
#include <set>

namespace RuamEngine
{
    using colliderPairKey = std::pair<Collider*, Collider*>;
    class PhysicsManager
    {
        static std::vector<BoxCollider*> s_boxColliders;
        static std::map<colliderPairKey, Collision> s_previousCollisions;
    public:
        static void AddRigidbody(Rigidbody* rb);
        static void AddBoxCollider(BoxCollider* box);

        static void RemoveRigidbody(Rigidbody* rb);
        static void RemoveBoxCollider(BoxCollider* box);

        static void Update();
    };
}
