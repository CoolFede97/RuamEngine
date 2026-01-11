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
	class GlobalLight : public BaseRenderer
	{

	private:
		static GlobalLight* s_mainLight;
		static Vec4 m_color;
		static float m_lightOffset;
	public:
		using BaseRenderer::BaseRenderer;
		~GlobalLight();
		static void setLightColor(Vec4 color);

		static void setLightOffset(float offset);

		void update(){};
		void start();

		void render(){};

		static void loadLightSettings(ShaderProgramSPtr program);

	};
}
