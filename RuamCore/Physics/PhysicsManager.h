#pragma once

#include "Collision.h"
#include "Rigidbody.h"
#include "BoxCollider.h"

#include <vector>
#include <set>

namespace RuamEngine
{
    using rbPairKey = std::pair<Rigidbody*, Rigidbody*>;
    class PhysicsManager
    {
        static std::vector<Rigidbody*> s_rigidbodies;
        static std::vector<BoxCollider*> s_boxColliders;
        static std::map<rbPairKey, Collision> s_previousCollisions;
    public:
        static void AddRigidbody(Rigidbody* rb);
        static void AddBoxCollider(BoxCollider* box);

        static void RemoveRigidbody(Rigidbody* rb);
        static void RemoveBoxCollider(BoxCollider* box);

        static void Update();
    };
}
