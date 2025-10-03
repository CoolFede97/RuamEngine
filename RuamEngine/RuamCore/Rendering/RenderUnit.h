#pragma once

#include "Material.h"
#include "RenderingElements.h"
#include "RenderingConstants.h"

namespace RuamEngine
{
    class RenderUnit
    {
    public:


	    RenderUnit(ShaderPtr& shader);
        RenderUnit();

	    // This shader must be the same as the drawingData that contains this renderUnit
	    ShaderPtr m_shader;
        MaterialPtr m_material = nullptr;
        VertexArrayPtr m_vertexArray = std::make_unique<VertexArray>();
        SSBOPointer<Vertex> m_vertices = std::make_unique<SSBO<Vertex>>(maxVertexCount, GL_DYNAMIC_STORAGE_BIT);
        SSBOPointer<unsigned int> m_indices = std::make_unique<SSBO<unsigned int>>(maxIndexCount, GL_DYNAMIC_STORAGE_BIT);
        SSBOPointer<glm::mat4> m_modelMatricesBuffer = std::make_unique<SSBO<glm::mat4>>(maxVertexCount, GL_DYNAMIC_STORAGE_BIT);
        VertexBufferLayoutPtr m_layout = std::make_unique<VertexBufferLayout>();
        IndexBufferPtr m_indexBuffer = std::make_unique<IndexBuffer>(maxIndexCount, GL_DYNAMIC_DRAW);

        void SubmitBatchData();
        bool AddBatchData(const std::vector<Vertex> vertices, unsigned int vertexDataSize, const std::vector<unsigned int> indices, unsigned int indexDataSize);
        bool AddBatchData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<glm::mat4>& modelMatrices);
        void Flush();
    private:
    };
}