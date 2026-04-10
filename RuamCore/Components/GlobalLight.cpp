#include "GlobalLight.h"
#include "RuamEngine.h"

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

	void GlobalLight::renderStart()
	{
		if (!s_mainLight) s_mainLight = this;
		else if (s_mainLight != this) entity()->removeComponent<GlobalLight>(); // PREGUNTARLE A TOMI COMO HACER QUE ESTO BORRE A �ESTE! COMPONENTE
		SetLightColor(m_color);
	}

	void GlobalLight::LoadLightSettings(ShaderProgramSPtr program)
	{
	    program->setUniform4f("u_globalLightColor", m_color.x, m_color.y, m_color.z, m_color.w);
		program->setUniform1f("u_lightOffset", m_lightOffset);
		program->setUniform3f("u_globalLightPos", s_mainLight->entity()->transform().position().x, s_mainLight->entity()->transform().position().y, s_mainLight->entity()->transform().position().z);
	}
	DEF_REGISTER_COMPONENT(GlobalLight)
}
