#pragma once

#include "Material.h"
#include <vector>
#include <Vertex.h>
namespace RuamEngine
{
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

		unsigned int GetInstanceId() const { return m_instanceId; }
	private:
		unsigned int m_instanceId;
		static unsigned int s_instanceCount;
	};
	using MeshSPtr = std::shared_ptr<Mesh>;
	using MeshWPtr = std::weak_ptr<Mesh>;
}
