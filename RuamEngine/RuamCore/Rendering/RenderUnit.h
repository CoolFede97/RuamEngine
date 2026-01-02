#pragma once

#include "Material.h"
#include "ShaderProgram.h"
#include "RenderingElements.h"
#include "RenderingConstants.h"

namespace RuamEngine
{
    class ShaderProgram;
    class DrawingData;
	using DrawingDataSPtr = std::shared_ptr<DrawingData>;

    class RenderUnit
    {
    public:

        RenderUnit();

	    // This shader must be the same as the drawingData that contains this renderUnit
	    DrawingDataSPtr m_drawingData = nullptr;
		ShaderProgramSPtr m_program = nullptr;
        MaterialWPtr m_material = {};
        VertexArrayPtr m_vertexArray = std::make_unique<VertexArray>();
        SSBOPointer<Vertex> m_vertices = std::make_unique<SSBO<Vertex>>(maxVertexCount, GL_DYNAMIC_STORAGE_BIT);
        SSBOPointer<unsigned int> m_indices = std::make_unique<SSBO<unsigned int>>(maxIndexCount, GL_DYNAMIC_STORAGE_BIT);
        SSBOPointer<glm::mat4> m_modelMatricesBuffer = std::make_unique<SSBO<glm::mat4>>(maxVertexCount, GL_DYNAMIC_STORAGE_BIT);
    	std::vector<unsigned int> m_meshesRegistered; // Stores all the meshes instance id

		bool m_staticStorage = false;
		bool m_staticPosition = false;

        void SubmitData();
		void BindBuffersBase();

        //bool AddBatchData(const std::vector<Vertex> vertices, unsigned int vertexDataSize, const std::vector<unsigned int> indices, unsigned int indexDataSize);
        bool AddBatchData(const std::vector<Vertex>& vertices, std::vector<unsigned int> indices, const std::vector<glm::mat4>& modelMatrices);
        bool AddModelMatrix(const std::vector<glm::mat4>& modelMatrices);
        void Flush();
    private:
        unsigned int m_indexCount = 0;
        bool m_uploaded = false;
    };
    using RenderUnitSPtr = std::shared_ptr<RenderUnit>;
    using RenderUnitWPtr = std::weak_ptr<RenderUnit>;
}
