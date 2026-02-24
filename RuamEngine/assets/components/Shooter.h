#pragma once

#include "Entity.h"
#include "../../RuamCore/Input/Input.h"
#include "RuamTime.h"

#include "glm/glm.hpp"
#include "ModelRenderer.h"

namespace RuamEngine
{
	#define SHOOTER_SERIALIZED_MEMBERS(X, ...) \
	X(m_bulletSpeed, float, 0.0f, Shooter)__VA_ARGS__ \
	X(m_bulletRadius, float, 1.0f, Shooter)__VA_ARGS__ \
	X(m_shootingInterval, float, 0.0f, Shooter)__VA_ARGS__ \
	X(m_bulletMeshPath, std::string, "", Shooter)__VA_ARGS__ \
	X(m_health, float, 100.0f, Shooter)__VA_ARGS__ \
	X(m_damage, float, 3.0f, Shooter)

	#define SHOOTER_STATIC_MEMBERS(X, ...) \
	X(s_instance, Shooter*, nullptr, Shooter)

	class Shooter : public Component
	{
		using Component::Component;


		void start();
		void update();

		float m_timeSinceLastShot = 0.0f;

	public:
		SHOOTER_STATIC_MEMBERS(DECL_STATIC_MEMBER)
		SHOOTER_SERIALIZED_MEMBERS(DECL_MEMBER)

		IMPL_forEachSerializedField(SHOOTER_SERIALIZED_MEMBERS(CALL_INSPECTOR_DRAWER))
		IMPL_SERIALIZE(Shooter, SHOOTER_SERIALIZED_MEMBERS(SER_FIELD, ,))

		std::string name() override { return "Shooter"; }

		void take_damage(float damage);
		~Shooter();
	};
}
