#pragma once

#include <Component.hpp>
#include "Renderer.h"
#include "Vertex.h"
#include "SSBO.h"


using namespace RuamEngine;

class SandboxCom2 : public BaseRenderer
{
	using BaseRenderer::BaseRenderer;

	// It's called in update
	void render()
	{
		
	};
	void update()
	{
		render();
	};

};
