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

public:
	IMPL_SIMPLE_SERIALIZE(SandboxCom)
	std::string name() override { return "SandboxCom"; }
	IMPL_DRAW_SERIALIZED_MEMBERS(;)

	void start()
	{

	}

	void render()
	{

	}

	void update()
	{
		transform().position().x+=30*Time::DeltaTime();
	}
};
