#pragma once

#include "Entity.h"
#include "Component.h"
namespace RuamEngine
{
    template<class Cmp>
    Cmp* Component::getComponent() const
    {
        return entity()->getComponent<Cmp>();
    }

    template<class Cmp>
    std::vector<Cmp*> Component::getComponentsOfType() const
    {
        return entity()->getComponentsOfType<Cmp>();
    }

    template<class Cmp>
    void Component::removeComponent()
    {
        entity()->removeComponent<Cmp>();
    }

    template<class Cmp>
    void Component::removeComponent(Component& comp)
    {
        entity()->removeComponent<Cmp>(comp);
    }
}
