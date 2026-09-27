#pragma once

#include "Plane.h"
#include "AABB.h"

namespace RuamEngine
{
    class Camera;
    struct Frustum
    {
        Frustum(); // Uses the static class Camera
        bool sphereInside(glm::vec3 center, float radius, Camera* camera);
        bool boxInside(AABB aabb, Camera* camera);
        Plane nearPlane;
        Plane farPlane;
        Plane leftPlane;
        Plane rightPlane;
        Plane topPlane;
        Plane bottomPlane;
        Plane* planes[6] =
        {
            &nearPlane,
            &farPlane,
            &leftPlane,
            &rightPlane,
            &topPlane,
            &bottomPlane
        };
    private:
        void update(Camera* camera);
    };
}
