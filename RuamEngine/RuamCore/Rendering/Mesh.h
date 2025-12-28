#pragma once

#include "Material.h"
#include <vector>
#include <Vertex.h>
namespace RuamEngine
{
	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, MaterialPtr material);
		~Mesh();

		Mesh(const Mesh& other);

		Mesh& operator=(const Mesh& other);

		Mesh(Mesh&& other) noexcept;

		Mesh& operator=(Mesh&& other) noexcept;

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		MaterialPtr m_material;
	};
	using MeshPtr = std::shared_ptr<Mesh>;
}
