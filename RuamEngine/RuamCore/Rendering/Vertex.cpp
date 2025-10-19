#include "Vertex.h"

namespace RuamEngine
{
	std::vector<Vertex> Vertex::CreateQuad(float size, float x, float y, float texId)
	{
		float half = size / 2;
		Vertex v0;
		v0.m_position = { x - half, y - half, 0.0f };
		v0.m_uv = {0.0f, 0.0f};
		v0.m_normal = { 0.0f, 0.0f, 1.0f };
		v0.m_texId = texId;
		Vertex v1;
		v1.m_position = { x + half, y - half, 0.0f };
		v1.m_uv = {1.0f,0.0f};
		v1.m_normal = { 0.0f, 0.0f, 1.0f };
		v1.m_texId = texId;
		Vertex v2;
		v2.m_position = { x + half, y + half, 0.0f };
		v2.m_uv = {1.0f,1.0f};
		v2.m_normal = { 0.0f, 0.0f, 1.0f };
		v2.m_texId = texId;
		Vertex v3;
		v3.m_position = { x - half, y + half, 0.0f };
		v3.m_uv = {0.0f, 1.0f};
		v3.m_normal = { 0.0f, 0.0f, 1.0f };
		v3.m_texId = texId;
		return { v0, v1, v2, v3 };
	}

	std::vector<Vertex> Vertex::CreateCube(float texId)
	{
		std::vector<Vertex> cube;

		// Face front (Z-)
		cube.push_back({ {-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, texId });
		cube.push_back({ { 1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, texId });
		cube.push_back({ { 1.0f,  1.0f, -1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, texId });
		cube.push_back({ {-1.0f,  1.0f, -1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, texId });

		// Face back (Z+)
		cube.push_back({ {-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, texId });
		cube.push_back({ { 1.0f, -1.0f,  1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, texId });
		cube.push_back({ { 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, texId });
		cube.push_back({ {-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, texId });

		// Face left (X-)
		cube.push_back({ {-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, texId });
		cube.push_back({ {-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, texId });
		cube.push_back({ {-1.0f,  1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, texId });
		cube.push_back({ {-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, texId });

		// Face right (X+)
		cube.push_back({ { 1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, texId });
		cube.push_back({ { 1.0f, -1.0f,  1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, texId });
		cube.push_back({ { 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, texId });
		cube.push_back({ { 1.0f,  1.0f, -1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, texId });

		// Face bottom (Y-)
		cube.push_back({ {-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, texId });
		cube.push_back({ { 1.0f, -1.0f,  1.0f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, texId });
		cube.push_back({ { 1.0f, -1.0f, -1.0f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, texId });
		cube.push_back({ {-1.0f, -1.0f, -1.0f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, texId });

		// Face top (Y+)
		cube.push_back({ {-1.0f,  1.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, texId });
		cube.push_back({ { 1.0f,  1.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, texId });
		cube.push_back({ { 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, texId });
		cube.push_back({ {-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, texId });

		return cube;

	}

	std::vector<float> Vertex::FlattenVertices(const std::vector<Vertex>& vertices)
	{
		std::vector<float> result;
		result.reserve(vertices.size() * (3 + 2 + 3 + 1));  // pos + uv + normal + texId

		for (const auto& vertex : vertices)
		{
			std::vector<float> flatVertex = vertex;
			result.insert(result.end(), flatVertex.begin(), flatVertex.end());
		}
		return result;
	}
}
