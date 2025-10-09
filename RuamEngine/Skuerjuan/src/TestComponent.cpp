#include "TestComponent.hpp"
#include <utility>
// namespace {
//     const bool registered = []() {
//         Component::componentRegistry.insert(std::make_pair(
//             "TestComponent", [](const nlohmann::json &j, Object *o) -> std::unique_ptr<Component> {
//                 return std::make_unique<TestComponent>(o->addComponent<TestComponent, const nlohmann::json&>(0, j));
//             }
//         ));
//         std::cout << "Registered TestComponent" << std::endl;
//         return true;
//     }();
// }
//TODO: fix addComponent issues