#include "Collision.h"
#include "Collider.h"
#include "Rigidbody.h"
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
        Collider* a = col.a;
        Collider* b = col.b;

        if (a->m_trigger || b->m_trigger) return;

        Rigidbody* rbA = a->getComponent<Rigidbody>();
        Rigidbody* rbB = b->getComponent<Rigidbody>();

        float invMassA = rbA ? 1 / rbA->m_mass : 0;
        float invMassB = rbB ? 1 / rbB->m_mass : 0;
        float invMassSum = invMassA + invMassB;

        if (invMassSum<=0) return;

        glm::vec3 correction = col.normal * col.penetration;

        float aInfluence= invMassA / invMassSum;
        float bInfluence = invMassB / invMassSum;

        a->transform()->translate(-correction * aInfluence);
        b->transform()->translate(correction * bInfluence);

        glm::vec3 deltaVelocity = (rbB ? rbB->m_velocity : glm::vec3(0,0,0)) - (rbA ? rbA->m_velocity : glm::vec3(0,0,0));
        float velAlongNormal = glm::dot(deltaVelocity, col.normal);

        if (velAlongNormal>0) return;

        float bounciness = ((rbA ? rbA->m_bounciness : 0) + (rbB ? rbB->m_bounciness : 0)) / 2;
        float force = (1 + bounciness) * velAlongNormal;

        if (rbA) rbA->m_velocity += force * col.normal * aInfluence;
        if (rbB) rbB->m_velocity -= force * col.normal * bInfluence;
    }
}
