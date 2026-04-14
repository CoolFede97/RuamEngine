#pragma once

#include "SceneManager.h"
#include "Scene.h"
#include "Entity.h"
#include <Component.h>

#include "ShaderProgram.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace RuamEngine
{
	class GlobalLight : public Component
	{

	private:
		static GlobalLight* s_mainLight;
		static Vec4 m_color;
		static float m_lightOffset;
	public:
		IMPL_SIMPLE_SERIALIZE(GlobalLight)
		using Component::Component;
		~GlobalLight();
		static void SetLightColor(Vec4 color);

		static void SetLightOffset(float offset);

		std::string name() const override { return "GlobalLight"; }

		void renderStart() override;

		void render(){};

		static void LoadLightSettings(ShaderProgramSPtr program);
	private:
		DECL_REGISTER_COMPONENT(GlobalLight);
	};
}
