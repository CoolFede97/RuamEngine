#pragma once

#include "Vertex.h"
#include "Material.h"
#include "RenderUnit.h"

namespace RuamEngine
{
	class Skybox
	{
	    static bool s_initialized;
		static GLuint m_cubemap;

		static std::vector<Vertex> m_vertices;
		static std::vector<unsigned int> m_indices;

		static MaterialWPtr m_material;
		static RenderUnitSPtr m_renderUnit;
	public:
		static void Init();
		static void SetSkybox(std::vector<std::string>& paths);
	};
}
