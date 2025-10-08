#include "Vertex.h"


std::array<Vertex, 4> Vertex::CreateQuad(float x, float y, Vec4 color)
{
	float size = 1.0f;
	float half = size / 2;
	Vertex v0;
	v0.m_position = { x - half, y - half, 0.0f };
	v0.m_color = color;
	Vertex v1;
	v1.m_position = { x + half, y - half, 0.0f };
	v1.m_color = color;
	Vertex v2;
	v2.m_position = { x + half, y + half, 0.0f };
	v2.m_color = color;
	Vertex v3;
	v3.m_position = { x - half, y + half, 0.0f };
	v3.m_color = color;
	return { v0, v1, v2, v3 };
}

std::array<VertexWithTex, 4> VertexWithTex::CreateQuad(float x, float y, float texID)
{
	float size = 1.0f;
	float half = size/2;

	VertexWithTex v0;
	v0.m_position = { x - half, y - half, 0.0f };
	v0.m_texCoords = { 0.0f, 0.0f };
	v0.m_tex_id = texID;

	VertexWithTex v1;
	v1.m_position = { x + half, y - half, 0.0f };
	v1.m_texCoords = { 1.0f, 0.0f };
	v1.m_tex_id = texID;

	VertexWithTex v2;
	v2.m_position = { x + half, y + half, 0.0f };
	v2.m_texCoords = { 1.0f, 1.0f };
	v2.m_tex_id = texID;

	VertexWithTex v3;
	v3.m_position = { x - half, y + half, 0.0f };
	v3.m_texCoords = { 0.0f, 1.0f };
	v3.m_tex_id = texID;

	return { v0, v1, v2, v3 };
}
