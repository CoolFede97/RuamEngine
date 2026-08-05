#pragma once

#include "glm/ext/vector_float3.hpp"
namespace RuamEngine
{
    struct Plane
    {
        Plane();
        Plane(glm::vec3 p_point, glm::vec3 p_normal);
        glm::vec3 normal;
        glm::vec3 point;
    };
}
