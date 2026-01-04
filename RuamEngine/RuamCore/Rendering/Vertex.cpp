#include "Vertex.h"

namespace RuamEngine
{
	std::vector<Vertex> Vertex::createQuad(float size, float x, float y, float texId)
	{
		float half = size / 2;
		Vertex v0;
		v0.m_position = { x - half, y - half, 0.0f };
		v0.m_uv = {0.0f, 0.0f};
		v0.m_normal = { 0.0f, 0.0f, 1.0f };
		Vertex v1;
		v1.m_position = { x + half, y - half, 0.0f };
		v1.m_uv = {1.0f,0.0f};
		v1.m_normal = { 0.0f, 0.0f, 1.0f };
		Vertex v2;
		v2.m_position = { x + half, y + half, 0.0f };
		v2.m_uv = {1.0f,1.0f};
		v2.m_normal = { 0.0f, 0.0f, 1.0f };
		Vertex v3;
		v3.m_position = { x - half, y + half, 0.0f };
		v3.m_uv = {0.0f, 1.0f};
		v3.m_normal = { 0.0f, 0.0f, 1.0f };
		return { v0, v1, v2, v3 };
	}

	std::vector<Vertex> Vertex::createCube()
	{
		std::vector<Vertex> cube;

		// Face front (Z-)
		cube.push_back({ {-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}});
		cube.push_back({ { 1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}});
		cube.push_back({ { 1.0f,  1.0f, -1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}});
		cube.push_back({ {-1.0f,  1.0f, -1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}});

		// Face back (Z+)
		cube.push_back({ {-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});
		cube.push_back({ { 1.0f, -1.0f,  1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});
		cube.push_back({ { 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});
		cube.push_back({ {-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});

		// Face left (X-)
		cube.push_back({ {-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}});
		cube.push_back({ {-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}});
		cube.push_back({ {-1.0f,  1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}});
		cube.push_back({ {-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}});

		// Face right (X+)
		cube.push_back({ { 1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
		cube.push_back({ { 1.0f, -1.0f,  1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
		cube.push_back({ { 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});
		cube.push_back({ { 1.0f,  1.0f, -1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});

		// Face bottom (Y-)
		cube.push_back({ {-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}});
		cube.push_back({ { 1.0f, -1.0f,  1.0f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}});
		cube.push_back({ { 1.0f, -1.0f, -1.0f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}});
		cube.push_back({ {-1.0f, -1.0f, -1.0f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}});

		// Face top (Y+)
		cube.push_back({ {-1.0f,  1.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}});
		cube.push_back({ { 1.0f,  1.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}});
		cube.push_back({ { 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}});
		cube.push_back({ {-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}});

		return cube;

	}

	std::vector<float> Vertex::flattenVertices(const std::vector<Vertex>& vertices)
	{
		std::vector<float> result;
		result.reserve(vertices.size() * (3 + 2 + 3));  // pos + uv + normal

		for (const auto& vertex : vertices)
		{
			std::vector<float> flatVertex = vertex;
			result.insert(result.end(), flatVertex.begin(), flatVertex.end());
		}
		return result;
	}
}
