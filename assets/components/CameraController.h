#pragma once

#include "Component.h"
#include "Vec3.h"
#include "Vec2.h"

using namespace RuamEngine;

namespace RuamEngine
{
	class CameraController : public Component
	{
		using Component::Component;

		Vec3 m_direction;
		Vec3 m_horizontalDirection;
		Vec2 m_mouseRotation;
	public:
	    float m_speed = 0;
		float m_rotationSpeed = 0;
		std::string name() const override { return "CameraController"; }
		private:
		void update() override;
	};
}
