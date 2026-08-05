#include "AABB.h"

namespace RuamEngine
{
    AABB TransformAABB(const AABB& aabb, const glm::mat4& transform)
    {
        AABB transformedAABB;

        glm::vec3 corners[8] =
        {
            {aabb.min.x, aabb.min.y, aabb.min.z}, {aabb.max.x, aabb.min.y, aabb.min.z},
            {aabb.min.x, aabb.max.y, aabb.min.z}, {aabb.max.x, aabb.max.y, aabb.min.z},
            {aabb.min.x, aabb.min.y, aabb.max.z}, {aabb.max.x, aabb.min.y, aabb.max.z},
            {aabb.min.x, aabb.max.y, aabb.max.z}, {aabb.max.x, aabb.max.y, aabb.max.z}
        };

        transformedAABB.min = glm::vec3(FLT_MAX);
        transformedAABB.max = glm::vec3(FLT_MIN);

        for (int i = 0; i < 8; i++)
        {
            glm::vec3 transformedVertex = glm::vec3(transform * glm::vec4(corners[i], 1.0f));
            transformedAABB.min = glm::min(transformedAABB.min, transformedVertex);
            transformedAABB.max = glm::max(transformedAABB.max, transformedVertex);
        }
        return transformedAABB;
    }
}
