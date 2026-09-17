#include "Collision.h"
#include "Transform.h"

namespace RuamEngine
{
    bool CheckAABBOverlap(BoxCollider* boxA, BoxCollider* boxB, glm::vec3& outNormal, float& outPenetration)
    {
        if (!boxA) {std::cerr << "Error: boxA is nullptr in a call to function CheckAABBOverlap\n"; return false;}
        if (!boxB) {std::cerr << "Error: boxB is nullptr in a call to function CheckAABBOverlap\n"; return false;}

        glm::vec3 posA = boxA->transform()->position() + boxA->m_center;
        glm::vec3 posB = boxB->transform()->position() + boxB->m_center;
        glm::vec3 delta = posB - posA;

        glm::vec3 scaleA = boxA->transform()->scale();
        glm::vec3 halfSizeA = boxA->m_halfSize * scaleA;
        glm::vec3 scaleB = boxB->transform()->scale();
        glm::vec3 halfSizeB = boxB->m_halfSize * scaleB;

        float overlapX = (halfSizeA.x + halfSizeB.x) - std::abs(delta.x);
        float overlapY = (halfSizeA.y + halfSizeB.y) - std::abs(delta.y);
        float overlapZ = (halfSizeA.z + halfSizeB.z) - std::abs(delta.z);

        if (overlapX<=0 || overlapY<=0 || overlapZ <= 0) return false;

        if (overlapX < overlapY && overlapX < overlapZ)
        {
            outPenetration = overlapX;
            outNormal = glm::vec3(delta.x >= 0 ? 1 : -1, 0, 0);
        }
        else if (overlapY < overlapZ)
        {
            outPenetration = overlapY;
            outNormal = glm::vec3(0, delta.y >= 0 ? 1 : -1, 0);
        }
        else
        {
            outPenetration = overlapZ;
            outNormal = glm::vec3(0, 0, delta.z >= 0 ? 1 : -1);
        }

        return true;
    }

    void ResolveCollision(Collision& col)
    {
        Rigidbody* a = col.a;
        Rigidbody* b = col.b;

        float invMassA = a->m_isKinematic ? 0 : 1 / a->m_mass;
        float invMassB = b->m_isKinematic ? 0 : 1 / b->m_mass;
        float invMassSum = invMassA + invMassB;

        if (invMassSum<=0) return;

        // std::cout << "Penetration: " << col.penetration << "\n";
        glm::vec3 correction = col.normal * col.penetration;

        float aInfluence= invMassA / invMassSum;
        float bInfluence = invMassB / invMassSum;

        if (!a->m_isKinematic) a->transform()->translate(-correction * aInfluence);
        if (!b->m_isKinematic) b->transform()->translate(correction * bInfluence);

        glm::vec3 deltaVelocity = b->m_velocity - a->m_velocity;
        float velAlongNormal = glm::dot(deltaVelocity, col.normal);

        if (velAlongNormal>0) return;

        float restitution = 1.0f;
        float force = (1 + restitution) * velAlongNormal;

        a->m_velocity += force * col.normal * aInfluence;
        b->m_velocity -= force * col.normal * bInfluence;
    }
}
