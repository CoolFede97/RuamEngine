#pragma once

#include "Material.h"
#include "Object.hpp"
#include "RuamTime.h"

#include "Mesh.h"

using namespace RuamEngine;

class ColorChanger : public Component {
	IMPL_SIMPLE_SERIALIZE(ColorChanger)

	using Component::Component;

	ColorChanger(unsigned int obj_id)
		: Component(obj_id) {}
	ColorChanger(const nlohmann::json& j, unsigned int obj_id) : Component(obj_id) {}

	void start()
	{
	}

	void update()
	{
	    m_time += Time::DeltaTime() * 3.0f;

		m_red   = (sin(m_time * 3.1f) * 0.5f) + 0.5f;
        m_green = (sin(m_time * 4.7f + 1.0f) * 0.5f) + 0.5f;
        m_blue  = (sin(m_time * 6.2f + 2.0f) * 0.5f) + 0.5f;

        for (auto& mesh : *m_meshes)
	    mesh.m_material->baseColor = Vec4(m_red, m_green, m_blue, 1.0f);
	}
private:
	float m_red = 0;
	float  m_green = 0;
	float m_blue = 0;
	float m_time = 0;
public:
    std::vector<Mesh>* m_meshes;

};

REGISTER_COMPONENT(ColorChanger)
