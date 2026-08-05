#include "Frustum.h"

#include "Camera.h"

#include <string>
namespace RuamEngine
{
    Frustum::Frustum() {}
    void Frustum::update(Camera* camera)
    {
        glm::vec3 nearPlanePoint = camera->position() + camera->front()* camera->nearPlane();
        nearPlane = {nearPlanePoint, camera->front()};

        glm::vec3 farPlanePoint = camera->position()+camera->front()*camera->farPlane();
        farPlane = {farPlanePoint, camera->back()};

        float radiansHalfFOV = glm::radians(camera->fov()/2);
        float farPlaneHalfHeight = glm::tan(radiansHalfFOV) * camera->farPlane();
        float farPlaneHalfWidth = farPlaneHalfHeight * camera->aspectRatio();

        glm::vec3 leftPlanePoint = farPlanePoint+farPlaneHalfWidth*camera->left();
        glm::vec3 leftPlaneNormal = glm::normalize(glm::cross(camera->up(), leftPlanePoint-camera->position()));
        leftPlane = { leftPlanePoint, leftPlaneNormal };

        glm::vec3 rightPlanePoint = farPlanePoint+farPlaneHalfWidth*camera->right();
        glm::vec3 rightPlaneNormal = glm::normalize(glm::cross(rightPlanePoint-camera->position(), camera->up()));
        rightPlane = {rightPlanePoint, rightPlaneNormal};

        glm::vec3 bottomPlanePoint = farPlanePoint+farPlaneHalfHeight*camera->down();
        glm::vec3 bottomPlaneNormal = glm::normalize(glm::cross(camera->left(), bottomPlanePoint-camera->position()));
        bottomPlane = { bottomPlanePoint, bottomPlaneNormal };

        glm::vec3 topPlanePoint = farPlanePoint+farPlaneHalfHeight*camera->up();
        glm::vec3 topPlaneNormal = glm::normalize(glm::cross(camera->right(), topPlanePoint-camera->position()));
        topPlane = {topPlanePoint, topPlaneNormal};
    }
    bool Frustum::sphereInside(glm::vec3 center, float radius, Camera* camera)
    {
        update(camera);

        for(const Plane* plane : planes)
        {
            float distance =
            glm::dot(
                plane->normal,
                center - plane->point
            );

            if(distance < -radius) return false;
        }

        return true;
    }
    bool Frustum::boxInside(AABB aabb, Camera* camera)
    {
        update(camera);
        for (const Plane* plane : planes)
        {
            glm::vec3 point;
            point.x = plane->normal.x>0 ? aabb.max.x : aabb.min.x;
            point.y = plane->normal.y>0 ? aabb.max.y : aabb.min.y;
            point.z = plane->normal.z>0 ? aabb.max.z : aabb.min.z;

            float distance =
            glm::dot(
                plane->normal,
                point - plane->point
            );

            if (distance < 0) return false;
        }
        return true;
    }
}
