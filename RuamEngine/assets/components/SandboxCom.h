#pragma once

#include "Component.h"
#include "Renderer.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Vertex.h"
#include "SSBO.h"
#include "SandboxCom2.h"

using namespace RuamEngine;

class SandboxCom : public BaseRenderer
{
	using BaseRenderer::BaseRenderer;

	void start()
	{

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
