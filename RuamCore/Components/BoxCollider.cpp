#include "BoxCollider.h"
#include "Component.h"
#include "GizmosManager.h"
#include "ModelRenderer.h"
#include "PhysicsManager.h"
#include "Entity.h"
#include "SceneManager.h"
#include "Transform.h"

namespace RuamEngine
{
    BoxCollider::BoxCollider(unsigned int entityId) : Component(entityId), m_center(0,0,0), m_halfSize(0.5, 0.5, 0.5)
    {
        PhysicsManager::AddBoxCollider(this);
        if (SceneManager::ActiveScene()) GetAndSetWithModelRenderer();
    }

    BoxCollider::~BoxCollider()
    {
        PhysicsManager::RemoveBoxCollider(this);
    }

    void BoxCollider::renderUpdate()
    {
        m_vertices.clear();
        glm::vec3 boxCenter = transform()->position() + m_center;
        glm::vec3 boxScale = transform()->scale();
        glm::vec3 halfX = glm::vec3(m_halfSize.x * boxScale.x, 0, 0);
        glm::vec3 halfY = glm::vec3(0, m_halfSize.y * boxScale.y, 0);
        glm::vec3 halfZ = glm::vec3(0, 0, m_halfSize.z * boxScale.z);

        m_vertices.push_back({boxCenter - m_halfSize});
        m_vertices.push_back({boxCenter + halfX - halfY - halfZ});
        m_vertices.push_back({boxCenter + halfX - halfY + halfZ});
        m_vertices.push_back({boxCenter - halfX - halfY + halfZ});
        m_vertices.push_back({boxCenter - halfX + halfY - halfZ});
        m_vertices.push_back({boxCenter + halfX + halfY - halfZ});
        m_vertices.push_back({boxCenter + m_halfSize});
        m_vertices.push_back({boxCenter - halfX + halfY + halfZ});

        GizmosManager::PushColliderGizmo(m_vertices, m_indices);
    }

    void BoxCollider::GetAndSetWithModelRenderer()
    {
        ModelRenderer* mr = entity()->getComponent<ModelRenderer>();
        if (mr && mr->m_model)
        {
            const AABB& aabb = mr->m_model->aabb();
            m_halfSize.x = (aabb.max.x - aabb.min.x)/2;
            m_halfSize.y = (aabb.max.y - aabb.min.z)/2;
            m_halfSize.z = (aabb.max.z - aabb.min.y)/2;
        }
    }

    DEF_REGISTER_COMPONENT(BoxCollider);
}
