#include "Collisions.h"

#include "Vec3.h"


AABB getAABB(const glm::vec3& vertices[]) {
    AABB aabb;
    for (int i = 0; i < 8; i++) {
        aabb.yMax = std::max(aabb.yMax, vertices[i]);

    }
    return aabb;
}

// bool AABBtoAABB(const AABB& box1, const AABB& box2)
// {
//     // Check for overlap on the x-axis
//     bool overlapX = box1.max.x >= box2.min.x && box2.max.x >= box1.min.x;
//
//     // Check for overlap on the y-axis
//     bool overlapY = box1.max.y >= box2.min.y && box2.max.y >= box1.min.y;
//
//     // Check for overlap on the z-axis
//     bool overlapZ = box1.max.z >= box2.min.z && box2.max.z >= box1.min.z;
//
//     // If there is overlap on all three axes, the AABBs are colliding
//     return overlapX && overlapY && overlapZ;
// }