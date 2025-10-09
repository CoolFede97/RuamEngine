#pragma once
#include "Serial.hpp"

class TestComponent : public Component {
public:
    using Component::Component;

    TestComponent(const nlohmann::json& j) : Component(j["id"]) {
        secret = j["secret"];
    }

    void start() {
        std::cout << "TestComponent started on object " << std::endl;
    }
    void update() {
        std::cout << "TestComponent updated on object " << std::endl;
    }

    int getSecret() const {return secret;}
    void setSecret(const int s) {secret = s;}

    operator nlohmann::json() const {
        return nlohmann::json{
				    {"type", "TestComponent"},
                        {"id", m_id},
                        {"secret", secret}
        };
    }

private:
    int secret;
};