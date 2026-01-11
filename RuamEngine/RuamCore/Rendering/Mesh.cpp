#include "Mesh.h"
#include "Material.h"
#include "ResourceManager.h"
#include "ResourceManager.h"
#include "RuamUtils.h"
#include <algorithm>
#include <memory>

namespace RuamEngine
{
	unsigned int Mesh::s_idCount = 0;

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, MaterialWPtr material)
		: m_vertices(vertices), m_indices(indices), m_material(material), m_id(s_idCount++)
	{
		if (!material.lock()) ResourceManager::GetMaterial(material.lock()->id());
	}

	Mesh::~Mesh()
	{
		ResourceManager::DestroyMaterial(m_material.lock()->id());
	}

	Mesh::Mesh(const Mesh& other)
	: m_vertices(other.m_vertices), m_indices(other.m_indices)
	{
		MaterialSPtr otherSharedMaterial = GetShared(other.m_material);
		if (otherSharedMaterial)
		{
			m_material = ResourceManager::GetMaterial(otherSharedMaterial->id());
		}
	}

	Mesh& Mesh::operator=(const Mesh& other)
	{
		if (this == &other) std::cerr << "Warning: A mesh is being created and assigned to itself\n";
		else
		{
			MaterialSPtr thisSharedMaterial = GetShared(m_material);
			MaterialSPtr otherSharedMaterial = GetShared(other.m_material);

			if (thisSharedMaterial) ResourceManager::DestroyMaterial(thisSharedMaterial->id());
				m_vertices = other.m_vertices;
				m_indices = other.m_indices;

			if (otherSharedMaterial) m_material = ResourceManager::GetMaterial(otherSharedMaterial->id());
		}
		return *this;
	}

	Mesh::Mesh(Mesh&& other) noexcept
	: m_vertices(std::move(other.m_vertices)), m_indices(std::move(other.m_indices)),
		m_material(std::move(other.m_material))
	{
		other.m_material = {};
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		if (this != &other)
		{
			MaterialSPtr sharedMaterial = GetShared(m_material);
			if (sharedMaterial) ResourceManager::DestroyMaterial(sharedMaterial->id());
			m_vertices = std::move(other.m_vertices);
			m_indices = std::move(other.m_indices);
			m_material = std::move(other.m_material);

			other.m_material = {};
		}
		return *this;
	}
}
