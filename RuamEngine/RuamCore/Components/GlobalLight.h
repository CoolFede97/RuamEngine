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
	class GlobalLight : public Component
	{
		IMPL_SIMPLE_SERIALIZE(GlobalLight)
		using Component::Component;
		GlobalLight(const nlohmann::json& j, unsigned int obj_id) : Component(obj_id) {}

	private:
		static GlobalLight* s_mainLight;
		Vec4 m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float m_lightOffset = 0.1f;
	public:

		void SetLightColor(Vec4 color)
		{
			m_color = color;
			Renderer::m_drawingDatas[0]->m_program->SetUniform4f("u_globalLightColor", m_color.x, m_color.y, m_color.z, m_color.w);
		}

		void SetLightOffset(float offset)
		{
			m_lightOffset = offset;
			Renderer::m_drawingDatas[0]->m_program->SetUniform1f("u_lightOffset", m_lightOffset);
		}

		void update() { Component::update(); };
		void start()
		{
			if (!s_mainLight) s_mainLight = this;
			else if (s_mainLight != this) object()->removeComponent<GlobalLight>(); // PREGUNTARLE A TOMI COMO HACER QUE ESTO BORRE A �ESTE! COMPONENTE
			SetLightColor(m_color);
			//SetLightOffset(m_lightOffset);
		}

		void render()
		{
			Renderer::m_drawingDatas[0]->m_program->Bind();
			Renderer::m_drawingDatas[0]->m_program->SetUniform3f("u_globalLightPos", object()->transform().position().x, object()->transform().position().y, object()->transform().position().z);
		}
	};
	REGISTER_COMPONENT(GlobalLight)
}
