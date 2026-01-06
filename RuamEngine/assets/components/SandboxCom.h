#pragma once

#include "Component.hpp"
#include "Renderer.h"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "Vertex.h"
#include "SSBO.h"
#include "SandboxCom2.h"

using namespace RuamEngine;

class SandboxCom : public BaseRenderer
{
	using BaseRenderer::BaseRenderer;

	void start()
	{
		Entity* newEntity = SceneManager::ActiveScene()->createEntity();
		std::cout << "START: SANDBOXCOM 1 UNO!\n";
		std::cout << "JUST CREATED COMPONENTS SIZE: " << SceneManager::ActiveScene()->m_justCreatedComponents.size() << "\n";
		newEntity->addComponent<SandboxCom2>();
		std::cout << "JUST CREATED COMPONENTS SIZE: " << SceneManager::ActiveScene()->m_justCreatedComponents.size() << "\n";

	}

	void render()
	{

	};
	void update()
	{
		std::cout << "UPDATE EN SANDBOX 1 UNO!\n";
		BaseRenderer::update();

	};
};
