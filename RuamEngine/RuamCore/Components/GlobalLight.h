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
		static void setLightColor(Vec4 color);

		static void setLightOffset(float offset);

		std::string name() override { return "GlobalLight"; }

		void start() override;

		void render(){};

		static void loadLightSettings(ShaderProgramSPtr program);

	};
}
