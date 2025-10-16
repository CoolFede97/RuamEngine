#include "Object.hpp"
#include "Component.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "../components/Manager.h"
#include "../components/Counter.h"
#include "../components/SandboxCom.h"
#include "../components/CubeRenderer.h"
#include "AudioSource.h"
#include "Camera.h"

Scene* CreateCollisionSandboxScene()
{
    Scene* testScene = SceneManager::CreateScene(2, "CollisionSandboxScene");
    Object* manager = testScene->newObject();
    Object* cube = testScene->newObject();
    Object* camera = testScene->newObject();
    cube->addComponent<CubeRenderer>();
    manager->addComponent<Manager>();
    camera->addComponent<RuamEngine::Camera>();
    cube->transform().setPosition(glm::vec3(0.0f, 0.0f, 5.5f));

    return testScene;
}
