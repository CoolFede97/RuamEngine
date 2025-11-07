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
		using Component::Component;
	
	private:
		static GlobalLight* s_mainLight;
		float m_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	public:

		void SetLightColor(Vec4 color)
		{
			m_color[0] = color.x;
			m_color[1] = color.y;
			m_color[2] = color.z;
			m_color[3] = color.w;
			Renderer::m_drawingDatas[0]->m_program->SetUniform4f("u_globalLightColor", m_color[0], m_color[1], m_color[2], m_color[3]);
		}

		void update() { Component::update(); };
		void start() 
		{
			if (!s_mainLight) s_mainLight = this;
			else if (s_mainLight != this) object()->removeComponent<GlobalLight>(); // PREGUNTARLE A TOMI COMO HACER QUE ESTO BORRE A ¡ESTE! COMPONENTE
			Renderer::m_drawingDatas[0]->m_program->SetUniform4f("u_globalLightColor", m_color[0], m_color[1], m_color[2], m_color[3]);
		}

		void render()
		{
			Renderer::m_drawingDatas[0]->m_program->Bind();
			Renderer::m_drawingDatas[0]->m_program->SetUniform3f("u_globalLightPos", object()->transform().position().x, object()->transform().position().y, object()->transform().position().z);
		}
	};
}