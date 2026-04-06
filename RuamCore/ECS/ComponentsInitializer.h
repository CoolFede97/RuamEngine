#pragma once
#include <iostream>

namespace RuamEngine
{
    class ComponentsInitializer
    {
        // Makes sure to register every component despite not being instanced
        static void InitComponents();
        friend class Engine;
    };
}
