#pragma once

#include "Component.h"
#include "Renderer.h"
#include "Vertex.h"
#include "SSBO.h"


using namespace RuamEngine;

class SandboxCom2 : public BaseRenderer
{
	using BaseRenderer::BaseRenderer;

	void start()
	{
		std::cout << "START FROM SANDBOXCOM 2 DOS!\n";
	}

	void render()
	{

	};
	void update()
	{
		std::cout << "UPDATE EN SANDBOXCOM 2 DOS!\n";
		BaseRenderer::update();
	};

};
