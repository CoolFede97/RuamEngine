#include "Mesh.h"
#include "ResourceManager.h"
#include "ResourceManager.h"
#include <algorithm>

namespace RuamEngine
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, MaterialPtr material)
		: m_vertices(vertices), m_indices(indices), m_material(material)
	{

	}

	Mesh::~Mesh()
	{
		ResourceManager::DestroyMaterial(m_material->GetId());
	}

	Mesh::Mesh(const Mesh& other)
	: m_vertices(other.m_vertices), m_indices(other.m_indices)
	{
		if (other.m_material)
		{
			m_material = ResourceManager::GetMaterial(other.m_material->GetId());
		}
	}

	Mesh& Mesh::operator=(const Mesh& other)
	{
		if (this == &other) std::cerr << "Warning: A mesh is being created and assigned to itself\n";
		else
		{
			if (m_material) ResourceManager::DestroyMaterial(m_material->GetId());
				m_vertices = other.m_vertices;
				m_indices = other.m_indices;

			if (other.m_material) m_material = ResourceManager::GetMaterial(other.m_material->GetId());
		}
		return *this;
	}

	Mesh::Mesh(Mesh&& other) noexcept
	: m_vertices(std::move(other.m_vertices)), m_indices(std::move(other.m_indices)),
		m_material(std::move(other.m_material))
	{
		other.m_material = nullptr;
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		if (this != &other)
		{
			if (m_material) ResourceManager::DestroyMaterial(m_material->GetId());
			m_vertices = std::move(other.m_vertices);
			m_indices = std::move(other.m_indices);
			m_material = std::move(other.m_material);

			other.m_material = nullptr;
		}
		return *this;
	}
}
