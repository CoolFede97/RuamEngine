#pragma once

#include "Component.h"
#include "SSBO.h"
#include "Vec3.h"
#include "Vertex.h"
#include "RenderingConstants.h"

namespace RuamEngine
{
    class BoxCollider : public Component
    {
        using Component::Component;

    public:
        BoxCollider(unsigned int entityId);
        ~BoxCollider();

        glm::vec3 m_center;
        glm::vec3 m_halfSize;

        std::vector<FieldInfo> fields() override
        {
            return
            {
                makeFieldInfo<glm::vec3>("m_center", m_center),
                makeFieldInfo<glm::vec3>("m_halfSize", m_halfSize)
            };
        }

       	void renderUpdate() override;

    private:
        void GetAndSetWithModelRenderer();

        std::vector<GizmoVertex> m_vertices = {};
        std::vector<unsigned int> m_indices =
        {
            0, 1, 1, 2, 2, 3, 3, 0, // Bottom face
            4, 5, 5, 6, 6, 7, 7, 4, // Top face
            0, 4, 1, 5, 2, 6, 3, 7 // 4 lateral edges
        };

        DECL_REGISTER_COMPONENT(BoxCollider);
    };
}
