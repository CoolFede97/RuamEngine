#pragma once

#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Object.hpp"
#include <Component.hpp>

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
		IMPL_SIMPLE_SERIALIZE(GlobalLight)
		using BaseRenderer::BaseRenderer;
		GlobalLight(const nlohmann::json& j, unsigned int obj_id) : BaseRenderer(obj_id) {}
		~GlobalLight();
		static void SetLightColor(Vec4 color);

		static void SetLightOffset(float offset);

		void update(){};
		void start();

		void render(){};

		static void LoadLightSettings(ShaderProgramPtr program);

	};
	REGISTER_COMPONENT(GlobalLight)
}
