#include "Mesh.h"
#include "Material.h"
#include "ResourceManager.h"
#include "ResourceManager.h"
#include "RuamUtils.h"
#include "Vertex.h"
#include <memory>

namespace RuamEngine
{
	unsigned int Mesh::s_idCount = 0;

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, MaterialSPtr material)
		: m_material(material), m_id(s_idCount++)
	{
	    pushData(vertices, indices);
		submitData();
	}

	Mesh::~Mesh()
	{
	}


	void Mesh::submitData()
	{
		if (m_vertices->currentSize() > 0)
		{
			if (!m_uploaded)
			{
				m_vertices->submitData();
			}
		}
		if (m_indices->currentSize() > 0)
		{
			if (!m_uploaded)
			{
				m_indices->submitData();
			}
		}

		if (!m_uploaded && (m_vertices->currentSize() > 0 || m_indices->currentSize() > 0))
		{
			m_uploaded = true;
		}
	}

	void Mesh::bindBuffersBase()
	{
		m_vertices->bindBufferBase(SSBOType::vertices);
		m_indices->bindBufferBase(SSBOType::indices);
	}

	void Mesh::pushData(const std::vector<Vertex>& vertices, std::vector<unsigned int> indices)
	{
		for (unsigned int i = 0; i < indices.size() ; i++)
		{
			indices[i] += m_indexCount;
		}
		m_indexCount += vertices.size();

		m_vertices->pushData(vertices);

        m_indices->pushData(indices);
	}

	// Mesh::Mesh(const Mesh& other)
	// : m_vertices(other.m_vertices), m_indices(other.m_indices)
	// {
	// 	MaterialSPtr otherSharedMaterial = other.m_material;
	// 	if (otherSharedMaterial)
	// 	{
	// 		m_material = ResourceManager::GetMaterial(otherSharedMaterial->id());
	// 	}
	// }

	// Mesh& Mesh::operator=(const Mesh& other)
	// {
	// 	if (this == &other) std::cerr << "Warning: A mesh is being created and assigned to itself\n";
	// 	else
	// 	{
	// 		MaterialSPtr thisSharedMaterial = m_material;
	// 		MaterialSPtr otherSharedMaterial = other.m_material;

	// 		m_vertices = other.m_vertices;
	// 		m_indices = other.m_indices;

	// 		if (otherSharedMaterial) m_material = ResourceManager::GetMaterial(otherSharedMaterial->id());
	// 	}
	// 	return *this;
	// }

	// Mesh::Mesh(Mesh&& other) noexcept
	// : m_vertices(std::move(other.m_vertices)), m_indices(std::move(other.m_indices)),
	// 	m_material(std::move(other.m_material))
	// {
	// 	other.m_material = {};
	// }

	// Mesh& Mesh::operator=(Mesh&& other) noexcept
	// {
	// 	if (this != &other)
	// 	{
	// 		MaterialSPtr sharedMaterial = m_material;
	// 		m_vertices = std::move(other.m_vertices);
	// 		m_indices = std::move(other.m_indices);
	// 		m_material = std::move(other.m_material);

	// 		other.m_material = {};
	// 	}
	// 	return *this;
	// }
}
