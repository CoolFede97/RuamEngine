#include "Vertex.h"

namespace RuamEngine
{
	std::vector<Vertex> Vertex::CreateQuad(float size, float x, float y, float texId)
	{
		float half = size / 2;
		Vertex v0;
		v0.m_position = { x - half, y - half, 0.0f };
		v0.m_uv = {0.0f, 0.0f};
		v0.m_texId = texId;
		Vertex v1;
		v1.m_position = { x + half, y - half, 0.0f };
		v1.m_uv = {1.0f,0.0f};
		v1.m_texId = texId;
		Vertex v2;
		v2.m_position = { x + half, y + half, 0.0f };
		v2.m_uv = {1.0f,1.0f};
		v2.m_texId = texId;
		Vertex v3;
		v3.m_position = { x - half, y + half, 0.0f };
		v3.m_uv = {0.0f, 1.0f};
		v3.m_texId = texId;
		return { v0, v1, v2, v3 };
	}

	std::vector<Vertex> Vertex::CreateCube(float texId)
	{
		Vertex v0; v0.m_position = { -1.0f, -1.0f, -1.0f }; v0.m_uv = { 0.0f, 0.0f }; v0.m_texId = texId;
		Vertex v1; v1.m_position = { 1.0f, -1.0f, -1.0f }; v1.m_uv = { 1.0f, 0.0f }; v1.m_texId = texId;
		Vertex v2; v2.m_position = { 1.0f,  1.0f, -1.0f }; v2.m_uv = { 1.0f, 1.0f }; v2.m_texId = texId;
		Vertex v3; v3.m_position = { -1.0f,  1.0f, -1.0f }; v3.m_uv = { 0.0f, 1.0f }; v3.m_texId = texId;

		Vertex v4; v4.m_position = { -1.0f, -1.0f,  1.0f }; v4.m_uv = { 0.0f, 0.0f }; v4.m_texId = texId;
		Vertex v5; v5.m_position = { 1.0f, -1.0f,  1.0f }; v5.m_uv = { 1.0f, 0.0f }; v5.m_texId = texId;
		Vertex v6; v6.m_position = { 1.0f,  1.0f,  1.0f }; v6.m_uv = { 1.0f, 1.0f }; v6.m_texId = texId;
		Vertex v7; v7.m_position = { -1.0f,  1.0f,  1.0f }; v7.m_uv = { 0.0f, 1.0f }; v7.m_texId = texId;

		return { v0, v1, v2, v3, v4, v5, v6, v7 };
	}

	std::vector<float> Vertex::FlattenVertices(const std::vector<Vertex>& vertices)
	{
		std::vector<float> result;
		result.reserve(vertices.size() * (3 + 2 + 1));  // pos + uv + texId

		for (const auto& vertex : vertices)
		{
			std::vector<float> flatVertex = vertex;
			result.insert(result.end(), flatVertex.begin(), flatVertex.end());
		}
		return result;
	}
}

