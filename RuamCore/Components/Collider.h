#pragma once

#include "Component.h"

namespace RuamEngine
{
    class Collider : public Component
    {
        using Component::Component;

    public:
        Collider();
        std::string m_colliderName;
    };
}
