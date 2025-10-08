#pragma once

#include "RenderingCore.h"

namespace RuamEngine
{
	struct Vertex
	{
		Vec3 m_position;  // x, y, z	
		Vec2 m_uv;
		float m_texId = 0;
	 
		static std::vector<Vertex> CreateQuad(float size, float x, float y, float texId=0);
		static std::vector<Vertex> CreateCube(float texId = 0);
		operator std::vector<float>() const
		{
			return
			{ 
				m_position.x, m_position.y, m_position.z,
				m_uv.x, m_uv.y,
				m_texId
			};
		}
		static std::vector<float> FlattenVertices(const std::vector<Vertex>& vertices);
	};
}
