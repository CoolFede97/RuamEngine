#pragma once

#include "Component.h"
#include "Renderer.h"
#include "Vertex.h"
#include "SSBO.h"


using namespace RuamEngine;

class SandboxCom2 : public Component
{
	using Component::Component;

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
		Component::update();
	};

public:
	IMPL_SIMPLE_SERIALIZE(SandboxCom2)
	std::string name() override { return "SandboxCom2"; }
	IMPL_DRAW_SERIALIZED_MEMBERS(;)
};
