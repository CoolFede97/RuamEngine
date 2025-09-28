#pragma once

#include "Material.h"
#include "RenderingElements.h"
#include "RenderingConstants.h"


namespace RuamEngine
{
    class RenderUnit
    {
    public:


	    RenderUnit(ShaderPtr shader);
        RenderUnit();

	    // This shader must be the same as the drawingData that contains this renderUnit
	    ShaderPtr m_shader = nullptr;
        MaterialPtr m_material = nullptr;
        VertexArrayPtr m_vertexArray = std::make_shared<VertexArray>();
        VertexBufferPtr m_vertexBuffer = std::make_shared<VertexBuffer>(maxVertexSize * maxVertexCount, GL_DYNAMIC_DRAW);
        VertexBufferLayoutPtr m_layout = std::make_shared<VertexBufferLayout>();
        IndexBufferPtr m_indexBuffer = std::make_shared<IndexBuffer>(maxIndexCount, GL_DYNAMIC_DRAW);

        void SubmitBatchData();
        bool AddBatchData(const std::vector<float> vertices, unsigned int vertexDataSize, const std::vector<unsigned int> indices, unsigned int indexDataSize);
        void Flush();
    private:
    };
}