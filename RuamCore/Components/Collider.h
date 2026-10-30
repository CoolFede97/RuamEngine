#pragma once

#include "Component.h"

namespace RuamEngine
{
    class Collider : public Component
    {
        using Component::Component;

    public:
        Collider();
        bool m_trigger = false;
        std::string m_colliderName;

        std::vector<FieldInfo> fields() override
        {
            return
            {
                makeFieldInfo<bool>("m_trigger", m_trigger),
                makeFieldInfo<std::string>("m_colliderName", m_colliderName)
            };
        }
    };
}
