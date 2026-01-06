// #pragma once

// #include <Component.hpp>
// #include "RuamEngine.h"
// #include "SceneManager.hpp"
// #include "Scene.hpp"
// #include "RuamTime.h"

// using namespace RuamEngine;

// class Physics : public Component {
//     using Component::Component;

//     glm::vec3 velocity = glm::vec3(0.0f);
//     glm::vec3 acceleration = glm::vec3(0.0f);
//     float mass = 1.0f;
//     bool useGravity = true;
//     const glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
//     bool isGrounded = false;

//     void update() {
//         entity().transform().position() = entity().transform().position() + velocity;
//         if (useGravity && !isGrounded) {
//             velocity += gravity * Time::DeltaTime();
//         }
//     }
// }
