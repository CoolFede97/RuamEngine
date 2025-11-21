#include "Object.hpp"
#include "Component.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "../components/CameraController.h"
#include "Serial.hpp"
#include "MeshRenderer.h"
#include "GlobalLight.h"

#include "../components/ColorChanger.h"
void CreateWinScene()
{
	SceneManager::CreateScene(3, "WinScene");
	auto scene = SceneManager::ActiveScene();

	Object* light = scene->newObject();
	light->addComponent<GlobalLight>();
	light->transform().setPosition(glm::vec3(0.0f, 80.0f, 0.0f));
	//manager->addComponent<SandboxCom>();
	//bag->transform().setRotation(glm::vec3(45.0f, 5.0f, 45.0f));
	//bag->addComponent<CubeRenderer>()->materialId = 0;

	Object* nave = scene->newObject();
	nave->transform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	nave->addComponent<MeshRenderer>()->SetModel("assets/meshes/Nave/Nave.obj");
	MeshRenderer* meshRenderer = nave->addComponent<MeshRenderer>();
	meshRenderer->SetModel("assets/meshes/Nave/Nave.obj");

	Object* skybox = scene->newObject();
	skybox->addComponent<MeshRenderer>()->SetModel("assets/meshes/skyboxes/white/white.obj");
	skybox->getComponent<MeshRenderer>()->m_model->m_meshes[0].m_material->m_shininess = 1000.0;
	skybox->addComponent<ColorChanger>()->m_meshes = &skybox->getComponent<MeshRenderer>()->m_model->m_meshes;
	skybox->transform().setRotation(glm::vec3(180.0f, 0.0, 0.0));
	skybox->transform().setScale(glm::vec3(500.0f, 500.0f, 500.0f));

	Object* player = scene->newObject();
	player->addComponent<Camera>();
	player->addComponent<CameraController>();
	player->getComponent<CameraController>()->m_speed = 10.5f;
	player->getComponent<CameraController>()->m_rotationSpeed = 80.0f;
}
