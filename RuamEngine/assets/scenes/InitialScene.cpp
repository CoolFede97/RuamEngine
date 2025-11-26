#include "Object.hpp"
#include "Component.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "../components/CameraController.h"
#include "Serial.hpp"
#include "MeshRenderer.h"
#include "GlobalLight.h"
#include <AudioSource.h>

#include "../components/Manager.h"
void CreateInitialScene()
{
	SceneManager::CreateScene(1, "InitialScene");
	auto scene = SceneManager::ActiveScene();

	Object* light = scene->newObject();
	light->addComponent<GlobalLight>();
	light->transform().setPosition(glm::vec3(0.0f, 0.0f, 100.0f));
	light->setName("Light");
	//manager->addComponent<SandboxCom>();
	//bag->transform().setRotation(glm::vec3(45.0f, 5.0f, 45.0f));
	//bag->addComponent<CubeRenderer>()->materialId = 0;

	Object* nave = scene->newObject();
	nave->setName("Nave");
	nave->transform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	nave->addComponent<MeshRenderer>()->SetModel("assets/meshes/Nave/Nave.obj");

	Object* radio = scene->newObject();
	radio->addComponent<AudioSource>("assets/music/portal_radio.wav");
	radio->setName("radio");
	radio->transform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	radio->addComponent<MeshRenderer>()->SetModel("assets/meshes/radio/Radio.obj");
	radio->transform().setScale(0.002f, 0.002f, 0.002f);
	radio->transform().setPosition(glm::vec3(0.0f, -2.0f, 0.0f));
	radio->transform().setRotation(glm::vec3(90.0f, 0.0f, 0.0f));

	Object* portal = scene->newObject();
	portal->setName("portal");
	portal->transform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	portal->addComponent<MeshRenderer>()->SetModel("assets/meshes/portal/portal.obj");
	portal->transform().setScale(1.0f,1.0f,1.0f);
	portal->transform().setPosition(glm::vec3(0.0f, -2.0f, 20.0f));

	Object* skybox = scene->newObject();
	skybox->setName("Skybox");
	skybox->addComponent<MeshRenderer>()->SetModel("assets/meshes/skyboxes/galaxy/galaxySkybox.obj");
	skybox->getComponent<MeshRenderer>()->m_model->m_meshes[0].m_material->m_shininess = 1000.0;
	// skybox->addComponent<ColorChanger>()->m_meshes = &skybox->getComponent<MeshRenderer>()->m_model->m_meshes;
	skybox->transform().setRotation(glm::vec3(180.0f, 0.0, 0.0));
	skybox->transform().setScale(glm::vec3(500.0f, 500.0f, 500.0f));

	Object* player = scene->newObject();
	player->transform().setPosition(glm::vec3(100.0f, 30.0f, 150.0f));
	player->transform().setRotation(glm::vec3(0.0f, 180, 0.0f));
	player->setName("Player2");
	player->addComponent<Camera>();
	player->addComponent<CameraController>();
	player->getComponent<CameraController>()->m_speed = 10.5f;
	player->getComponent<CameraController>()->m_rotationSpeed = 10000.0f;

	Object* manager = scene->newObject();
	manager->setName("Manager");
	manager->addComponent<Manager>();
	Serial::serialise(SceneManager::ActiveScene());
}
