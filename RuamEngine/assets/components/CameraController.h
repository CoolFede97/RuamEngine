#pragma once

#include "Component.hpp"
#include "Object.hpp"
#include "Renderer.h"
#include "Vertex.h"
#include "SSBO.h"
#include "RuamTime.h"
using namespace RuamEngine;

class CameraController : public Component
{
	using Component::Component;

	void update()
	{
		Camera::GetMainCamera()->object()->transform().position() += glm::vec3(0, 0, Time::DeltaTime() * 100);
	};
	void start() {};
};
