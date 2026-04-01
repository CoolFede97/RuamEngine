#pragma once

#include "Material.h"
#include <vector>
namespace RuamEngine
{
    class Vertex;
	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, MaterialWPtr material);
		~Mesh();

		Mesh(const Mesh& other);

		Mesh& operator=(const Mesh& other);

		Mesh(Mesh&& other) noexcept;

		Mesh& operator=(Mesh&& other) noexcept;

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		MaterialWPtr m_material;

		unsigned int id() const { return m_id; }
	private:
		unsigned int m_id; // Instance id
		static unsigned int s_idCount;
	};
	using MeshSPtr = std::shared_ptr<Mesh>;
	using MeshWPtr = std::weak_ptr<Mesh>;
}
