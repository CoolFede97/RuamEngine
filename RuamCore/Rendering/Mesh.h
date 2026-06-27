#pragma once

#include "Material.h"
#include "VertexArray.h"
#include "SSBO.h"
#include "RenderingConstants.h"
#include "Vertex.h"

#include <vector>
namespace RuamEngine
{
	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, MaterialSPtr material);
		~Mesh();

		MaterialSPtr m_material;

		void submitData();
		void bindBuffersBase();
        void pushData(const std::vector<Vertex>& vertices, std::vector<unsigned int> indices);

        VertexArrayUPtr m_vertexArray = std::make_unique<VertexArray>();
        SSBOUPtr<Vertex> m_vertices = std::make_unique<SSBO<Vertex>>(baseVertexCount, GL_DYNAMIC_STORAGE_BIT);
        SSBOUPtr<unsigned int> m_indices = std::make_unique<SSBO<unsigned int>>(baseIndexCount, GL_DYNAMIC_STORAGE_BIT);

        unsigned int id() const { return m_id; }
	private:
		unsigned int m_id; // Instance id
		static unsigned int s_idCount;

		unsigned int m_indexCount = 0;
		bool m_uploaded = false;
	};
	using MeshSPtr = std::shared_ptr<Mesh>;
	using MeshWPtr = std::weak_ptr<Mesh>;
}
