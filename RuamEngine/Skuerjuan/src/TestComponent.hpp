#pragma once
#include "Serial.hpp"
#include "Component.hpp"

class TestComponent : public Component {
public:
    using Component::Component;

    TestComponent(const nlohmann::json& j, unsigned int obj_id) : Component(obj_id) {
        secret = j["secret"];
    }

    void onStart() {
        std::cout << "TestComponent started on object " << std::endl;
    }
    void onUpdate() {
        std::cout << "TestComponent updated on object " << std::endl;
    }

    void start() override {
        onStart();
    }
    void update() override {
        onUpdate();
    }

    int getSecret() const {return secret;}
    void setSecret(const int s) {secret = s;}

    operator nlohmann::json() const {
        return nlohmann::json{
				    {"type", "TestComponent"},
                        {"secret", secret}
        };
    }

private:
    int secret;
};