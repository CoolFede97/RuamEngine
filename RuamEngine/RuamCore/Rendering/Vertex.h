#pragma once

#include "RendererCore.h"

struct Vertex
{
	Vec3 m_position;  // x, y, z	
	Vec4 m_color;    // r, g, b, a
	static std::array<Vertex, 4> CreateQuad(float x = 0, float y = 0, Vec4 color = Vec4(0.49f, 0.55f, 0.52f, 1.0f));
};

struct VertexWithTex
{
	Vec3 m_position;  // x, y, z	
	Vec2 m_texCoords; // u, v
	float m_tex_id;
	static std::array<VertexWithTex, 4> CreateQuad(float x = 0, float y = 0, float texID = 0);
};

