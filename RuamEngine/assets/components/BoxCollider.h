// #pragma once

// #include <Component.hpp>
// #include "RuamEngine.h"
// #include "SceneManager.hpp"
// #include "Scene.hpp"
// #include "RuamTime.h"

// using namespace RuamEngine;

// class BoxCollider : public Component {
//     using Component::Component;

//     int size;
//     glm::vec3 originPos;
//     glm::vec3 verticesPos[8];




//     void start() {
//         size = 1.0f;
//         originPos = object()->transform().position();
//         verticesPos[0] = originPos + glm::vec3(-size / 2.0f, size / 2.0f, -size / 2.0f);
//         verticesPos[1] = originPos + glm::vec3(-size / 2.0f, -size / 2.0f, -size / 2.0f);
//         verticesPos[2] = originPos + glm::vec3(size / 2.0f, size / 2.0f, -size / 2.0f);
//         verticesPos[3] = originPos + glm::vec3(size / 2.0f, -size / 2.0f, -size / 2.0f);
//         verticesPos[4] = originPos + glm::vec3(-size / 2.0f, size / 2.0f, size / 2.0f);
//         verticesPos[5] = originPos + glm::vec3(-size / 2.0f, -size / 2.0f, size / 2.0f);
//         verticesPos[6] = originPos + glm::vec3(size / 2.0f, size / 2.0f, size / 2.0f);
//         verticesPos[7] = originPos + glm::vec3(size / 2.0f, -size / 2.0f, size / 2.0f);
//     }

//     void update()  {

//     }

// };
