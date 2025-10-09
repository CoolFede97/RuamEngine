#include "TestComponent.hpp"

namespace {
    const bool registered = []() {
        Component::componentRegistry.insert({"TestComponent", [](const nlohmann::json& j) { return std::make_unique<TestComponent>(j); }});
        std::cout << "Registered TestComponent" << std::endl;
        return true;
    }();
}
