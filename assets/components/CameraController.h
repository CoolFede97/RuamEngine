#pragma once

#include "Component.h"
#include "Vec3.h"
#include "Vec2.h"

using namespace RuamEngine;

namespace RuamEngine
{

	#define CAMERA_CONTROLLER_SERIALIZED_MEMBERS(X, ...)	\
	X(m_speed, float, 0, nullptr)__VA_ARGS__	\
	X(m_rotationSpeed, float, 0, nullptr)

	class CameraController : public Component
	{
		using Component::Component;

		Vec3 m_direction;
		Vec3 m_horizontalDirection;
		Vec2 m_mouseRotation;
	public:
		CAMERA_CONTROLLER_SERIALIZED_MEMBERS(DECL_MEMBER)
		IMPL_DRAW_SERIALIZED_MEMBERS(CAMERA_CONTROLLER_SERIALIZED_MEMBERS(CALL_INSPECTOR_DRAWER))
		IMPL_SERIALIZE(CameraController, CAMERA_CONTROLLER_SERIALIZED_MEMBERS(SER_FIELD,,))
		std::string name() const override { return "CameraController"; }
		private:
		void update() override;
	};
}
