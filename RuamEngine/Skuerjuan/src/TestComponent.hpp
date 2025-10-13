#pragma once
#include "Serial.hpp"
#include "Component.hpp"

CREATE_COMPONENT(TestComponent,
    {
        secret = j["secret"];
        name = j["name"];
    },
    {
        std::cout << "TestComponent with name: " << name << " started on object " << std::endl;
    },
    {
        std::cout << "TestComponent updated on object " << std::endl;
    }
)
FIELD(int, secret, Secret, 0)
FIELD(std::string, name, Name, "")
END_COMPONENT_ARGS(TestComponent, {"secret", secret}, {"name", name})