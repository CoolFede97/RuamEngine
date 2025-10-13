#pragma once
#include "Serial.hpp"
#include "Component.hpp"

CREATE_COMPONENT(TestComponent,
    secret = j["secret"];,
    std::cout << "TestComponent started on object " << std::endl;,
    std::cout << "TestComponent updated on object " << std::endl;
)
FIELD(int, secret, Secret)
END_COMPONENT(TestComponent, {"type", "TestComponent"}, {"id", m_id}, {"secret", secret})