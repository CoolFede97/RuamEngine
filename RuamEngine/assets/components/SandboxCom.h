#pragma once

#include "Component.h"
#include "Renderer.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Vertex.h"
#include "SSBO.h"
#include "SandboxCom2.h"
#include "RuamTime.h"

using namespace RuamEngine;

class SandboxCom : public Component
{
	using Component::Component;
	void start()
	{

	}

	void render()
	{

	};
	void update()
	{
		transform().position().x+=30*Time::DeltaTime();
	};
};
