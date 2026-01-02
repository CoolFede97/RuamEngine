#include "GlobalLight.h"

namespace RuamEngine
{
	GlobalLight* GlobalLight::s_mainLight = nullptr;
	Vec4 GlobalLight::m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	float GlobalLight::m_lightOffset = 0.1f;

	GlobalLight::~GlobalLight()
	{
        if (s_mainLight == this) s_mainLight = nullptr;
	}

	void GlobalLight::SetLightColor(Vec4 color)
	{
		m_color = color;
	}
	void GlobalLight::SetLightOffset(float offset)
	{
		m_lightOffset = offset;
	}

	void GlobalLight::start()
	{
		if (!s_mainLight) s_mainLight = this;
		else if (s_mainLight != this) object()->removeComponent<GlobalLight>(); // PREGUNTARLE A TOMI COMO HACER QUE ESTO BORRE A �ESTE! COMPONENTE
		SetLightColor(m_color);
	}

	void GlobalLight::LoadLightSettings(ShaderProgramSPtr program)
	{
	    program->SetUniform4f("u_globalLightColor", m_color.x, m_color.y, m_color.z, m_color.w);
		program->SetUniform1f("u_lightOffset", m_lightOffset);
		program->SetUniform3f("u_globalLightPos", s_mainLight->object()->transform().position().x, s_mainLight->object()->transform().position().y, s_mainLight->object()->transform().position().z);
	}
}
