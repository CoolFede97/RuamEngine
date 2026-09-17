#pragma once

#include "BoxCollider.h"
#include "Rigidbody.h"
#include "glm/glm.hpp"

namespace RuamEngine
{
    struct Collision
    {
        Rigidbody* a;
        Rigidbody* b;
        glm::vec3 normal;
        float penetration;
    };

    bool CheckAABBOverlap(BoxCollider* boxA, BoxCollider* boxB, glm::vec3& outNormal, float& outPenetration);
    void ResolveCollision(Collision& col);
}
