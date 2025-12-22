#include "Object.hpp"
#include "Component.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "../components/Manager.h"
#include "../components/Counter.h"
#include "../components/SandboxCom.h"
#include "CubeRenderer.h"
#include "AudioSource.h"
#include "Camera.h"
#include "Serial.hpp"

void CreateCollisionSandboxScene()
{
    // SceneManager::CreateScene(2, "CollisionSandboxScene");
    // auto collisionSandboxScene = SceneManager::ActiveScene();
    // Object* manager = collisionSandboxScene->newObject();
    // Object* cube = collisionSandboxScene->newObject();
    // Object* camera = collisionSandboxScene->newObject();
    // cube->addComponent<CubeRenderer>();
    // manager->addComponent<Manager>();
    // camera->addComponent<Camera>();
    // cube->transform().setPosition(glm::vec3(0.0f, 0.0f, 5.5f));
}
