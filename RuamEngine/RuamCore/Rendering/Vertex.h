#pragma once

#include "RenderingCore.h"

struct Vertex
{
	static const int attributesFloatCount = 3 + 4; // x, y and z from m_position. r, g, b and a from m_color
	Vec3 m_position;  // x, y, z	
	Vec4 m_color;    // r, g, b, a

	static std::vector<Vertex> CreateQuad(float size, float x = 0, float y = 0, Vec4 color = Vec4(0.49f, 0.55f, 0.52f, 1.0f));
	operator std::vector<float>() const
	{
		return
		{ m_position.x, m_position.y, m_position.z,
			m_color.x, m_color.y, m_color.z, m_color.w
		};
	}
	static std::vector<float> FlattenVertices(const std::vector<Vertex>& vertices);
};

struct VertexWithTex
{
	static const int attributesFloatCount = 3 + 2 + 1; // x, y and z from m_position. x and y from m_texCoords and id from m_tex_id 
	Vec3 m_position;  // x, y, z	
	Vec2 m_texCoords; // u, v
	float m_texId;

	static std::vector<VertexWithTex> CreateQuad(float size, float x = 0, float y = 0, float texID = 0);
	operator std::vector<float>() const
	{
		return
		{ 
			m_position.x, m_position.y, m_position.z,
			m_texCoords.x, m_texCoords.y, m_texId
		};
	}
	static std::vector<float> FlattenVertices(const std::vector<VertexWithTex>& vertices);
};

