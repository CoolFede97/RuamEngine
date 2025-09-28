#include "Vertex.h"


std::vector<Vertex> Vertex::CreateQuad(float size, float x, float y, Vec4 color)
{
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

std::vector<float> Vertex::FlattenVertices(const std::vector<Vertex>& vertices)
{
	std::vector<float> result;
	result.reserve(vertices.size() * attributesFloatCount);

	for (const auto& vertex : vertices)
	{
		std::vector<float> flatVertex = vertex;
		result.insert(result.end(), flatVertex.begin(), flatVertex.end());
	}
	return result;
}


std::vector<VertexWithTex> VertexWithTex::CreateQuad(float size, float x, float y, float texID)
{
	float half = size/2;

	VertexWithTex v0;
	v0.m_position = { x - half, y - half, 0.0f };
	v0.m_texCoords = { 0.0f, 0.0f };
	v0.m_texId = texID;

	VertexWithTex v1;
	v1.m_position = { x + half, y - half, 0.0f };
	v1.m_texCoords = { 1.0f, 0.0f };
	v1.m_texId = texID;

	VertexWithTex v2;
	v2.m_position = { x + half, y + half, 0.0f };
	v2.m_texCoords = { 1.0f, 1.0f };
	v2.m_texId = texID;

	VertexWithTex v3;
	v3.m_position = { x - half, y + half, 0.0f };
	v3.m_texCoords = { 0.0f, 1.0f };
	v3.m_texId = texID;

	return { v0, v1, v2, v3 };
}

std::vector<float> VertexWithTex::FlattenVertices(const std::vector<VertexWithTex>& vertices)
{
	std::vector<float> result;
	result.reserve(vertices.size() * attributesFloatCount);

	for (const auto& vertex : vertices)
	{
		std::vector<float> flatVertex = vertex;
		result.insert(result.end(), flatVertex.begin(), flatVertex.end());
	}
	return result;
}