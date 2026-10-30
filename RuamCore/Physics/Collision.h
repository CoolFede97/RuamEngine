#pragma once

#include "BoxCollider.h"
#include "Rigidbody.h"
#include "glm/glm.hpp"

namespace RuamEngine
{
    struct Collision
    {
        Collider* a;
        Collider* b;
        glm::vec3 normal;
        float penetration;
    };

    struct CollisionInfo // Used as parameter for OnCollisionX functions
    {
        Collider* other;
        glm::vec3 normal;
        float penetration;
    };

    bool CheckAABBOverlap(BoxCollider* boxA, BoxCollider* boxB, glm::vec3& outNormal, float& outPenetration);
    void ResolveCollision(Collision& col);
}
