#include "Entity.hpp"
#include "Component.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "../components/CameraController.h"
#include "Serial.hpp"
#include "ModelRenderer.h"
#include "GlobalLight.h"
#include "Skybox.h"
#include "../components/Portal.h"
#include "../components/Manager.h"
#include "AudioSource.h"
Scene* CreateEndScene()
{
	Scene* scene = new Scene(2, "EndScene");
	SceneManager::SetActiveScene(scene);

	Entity* light = scene->createEntity();
	light->addComponent<GlobalLight>();
	light->transform().setPosition(glm::vec3(0.0f, 100.0f, 0.0f));
	light->setName("Light");

	Entity* nave = scene->createEntity();
	nave->setName("Nave");
	nave->transform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	ModelRenderer* naveRenderer = nave->addComponent<ModelRenderer>();
	naveRenderer->m_shaderProgramType = ShaderProgramType::general;
	naveRenderer->setModel("assets/meshes/Nave/Nave.obj");

	Entity* radio = scene->createEntity();
	radio->addComponent<AudioSource>("assets/music/portal_radio.wav");
	radio->setName("radio");
	radio->transform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	ModelRenderer* radioRenderer = radio->addComponent<ModelRenderer>();
	radioRenderer->m_shaderProgramType = ShaderProgramType::general;
	radioRenderer->setModel("assets/meshes/radio/Radio.obj");
	radio->transform().setScale(0.002f, 0.002f, 0.002f);
	radio->transform().setPosition(glm::vec3(0.0f, -2.0f, 0.0f));
	radio->transform().setRotation(glm::vec3(90.0f, 0.0f, 0.0f));

	Entity* portal = scene->createEntity();
	portal->setName("portal");
	portal->transform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	ModelRenderer* portalRenderer = portal->addComponent<ModelRenderer>();
	portalRenderer->m_shaderProgramType = ShaderProgramType::general;
	portalRenderer->setModel("assets/meshes/portal/portal.obj");
	portal->transform().setScale(1.0f,1.0f,1.0f);
	portal->transform().setPosition(glm::vec3(-4.0f, -2.6f, 35.0f));

	portal->addComponent<Portal>();

	Entity* skybox = scene->createEntity();
	skybox->setName("Skybox");
	skybox->transform().setScale(glm::vec3(50,50,50));
	skybox->addComponent<Skybox>();
	// skybox->addComponent<ModelRenderer>()->setModel("assets/meshes/skyboxes/galaxy/galaxySkybox.obj");
	// skybox->getComponent<ModelRenderer>()->m_model->m_meshes[0].m_material->m_shininess = 1000.0;
	// // skybox->addComponent<ColorChanger>()->m_meshes = &skybox->getComponent<ModelRenderer>()->m_model->m_meshes;
	// skybox->transform().setRotation(glm::vec3(180.0f, 0.0, 0.0));
	// skybox->transform().setScale(glm::vec3(500.0f, 500.0f, 500.0f));

	Entity* player = scene->createEntity();
	player->transform().setPosition(glm::vec3(-4.0f, 0.0f, 33.0f));
	player->transform().setRotation(glm::vec3(0.0f, 180, 0.0f));
	player->setName("Player2");
	player->addComponent<Camera>();
	player->addComponent<CameraController>();
	player->getComponent<CameraController>()->m_speed = 10.5f;
	player->getComponent<CameraController>()->m_rotationSpeed = 80.0f;

	Entity* manager = scene->createEntity();
	manager->setName("Manager");
	manager->addComponent<Manager>();
	// Serial::serialise(SceneManager::ActiveScene());

	return scene;
}
