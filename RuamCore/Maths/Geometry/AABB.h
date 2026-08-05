#pragma once
#include "glm/glm.hpp"
namespace RuamEngine
{
    struct AABB
    {
        glm::vec3 min = {INT_MAX, INT_MAX, INT_MAX};
        glm::vec3 max = {INT_MIN, INT_MIN, INT_MIN};
    };

    AABB TransformAABB(const AABB& aabb, const glm::mat4& transform);
}
