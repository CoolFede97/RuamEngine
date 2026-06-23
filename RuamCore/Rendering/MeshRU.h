#pragma once

#include "Material.h"
#include "RenderingCore.h"
#include "SSBO.h"
#include "ShaderProgram.h"
#include "RenderingConstants.h"
#include "Vertex.h"
#include "VertexArray.h"
#include <memory>

namespace RuamEngine
{
    class ShaderProgram;
    class DrawingData;
	using DrawingDataSPtr = std::shared_ptr<DrawingData>;
	using DrawingDataWPtr = std::weak_ptr<DrawingData>;

    class MeshRU
    {
    public:

        MeshRU(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

        MaterialWPtr m_material = {};
        VertexArrayUPtr m_vertexArray = std::make_unique<VertexArray>();
        SSBOUPtr<Vertex> m_vertices = std::make_unique<SSBO<Vertex>>(baseVertexCount, GL_DYNAMIC_STORAGE_BIT);
        SSBOUPtr<unsigned int> m_indices = std::make_unique<SSBO<unsigned int>>(baseIndexCount, GL_DYNAMIC_STORAGE_BIT);

        void submitData();
		void bindBuffersBase();

        void pushData(const std::vector<Vertex>& vertices, std::vector<unsigned int> indices);
    private:
        unsigned int m_indexCount = 0;
        bool m_uploaded = false;
    };
    using MeshRUSPtr = std::shared_ptr<MeshRU>;
    using MeshRUWPtr = std::weak_ptr<MeshRU>;
}
