#include "Vertex.h"

namespace RuamEngine
{
	std::vector<MeshVertex> MeshVertex::CreateQuad(float size, float x, float y, float texId)
	{
		float half = size / 2;
		MeshVertex v0;
		v0.m_position = { x - half, y - half, 0.0f };
		v0.m_uv = {0.0f, 0.0f};
		v0.m_normal = { 0.0f, 0.0f, 1.0f };
		MeshVertex v1;
		v1.m_position = { x + half, y - half, 0.0f };
		v1.m_uv = {1.0f,0.0f};
		v1.m_normal = { 0.0f, 0.0f, 1.0f };
		MeshVertex v2;
		v2.m_position = { x + half, y + half, 0.0f };
		v2.m_uv = {1.0f,1.0f};
		v2.m_normal = { 0.0f, 0.0f, 1.0f };
		MeshVertex v3;
		v3.m_position = { x - half, y + half, 0.0f };
		v3.m_uv = {0.0f, 1.0f};
		v3.m_normal = { 0.0f, 0.0f, 1.0f };
		return { v0, v1, v2, v3 };
	}

	std::vector<MeshVertex> MeshVertex::CreateCube()
	{
		std::vector<MeshVertex> cube;

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

	std::vector<float> MeshVertex::FlattenVertices(const std::vector<MeshVertex>& vertices)
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
