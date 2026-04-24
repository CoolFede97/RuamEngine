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

    class RenderUnit
    {
    public:

        RenderUnit();

	    // This shader must be the same as the drawingData that contains this renderUnit
	    DrawingDataWPtr m_drawingData = {};
		ShaderProgramSPtr m_program = nullptr;
        MaterialWPtr m_material = {};
        VertexArrayUPtr m_vertexArray = std::make_unique<VertexArray>();
        SSBOUPtr<Vertex> m_vertices = std::make_unique<SSBO<Vertex>>(maxVertexCount, GL_DYNAMIC_STORAGE_BIT);
        SSBOUPtr<unsigned int> m_indices = std::make_unique<SSBO<unsigned int>>(maxIndexCount, GL_DYNAMIC_STORAGE_BIT);
        SSBOUPtr<glm::mat4> m_modelMatricesBuffer = std::make_unique<SSBO<glm::mat4>>(maxVertexCount, GL_DYNAMIC_STORAGE_BIT);
    	std::vector<unsigned int> m_meshesRegistered; // Stores all the meshes instance id

		bool m_staticStorage = false;
		bool m_staticPosition = false;

        void submitData();
		void bindBuffersBase();

        //bool AddBatchData(const std::vector<Vertex> vertices, unsigned int vertexDataSize, const std::vector<unsigned int> indices, unsigned int indexDataSize);
        void addBatchData(const std::vector<Vertex>& vertices, std::vector<unsigned int> indices, const std::vector<glm::mat4>& modelMatrices);
        void addModelMatrices(const std::vector<glm::mat4>& modelMatrices);
        void flush();
    private:
        template<typename T>
        void resizeSSBO(SSBOUPtr<T>& ssbo, bool oneShotAmplifier) // doubles buffer size
        {
            unsigned int amplifier = oneShotAmplifier ? ssboOneShotCapacityAmplifier : ssboCapacityAmplifier;
            SSBOUPtr<T> newSSBO = std::make_unique<SSBO<T>>((ssbo->maxSize()/sizeof(T))*amplifier, GL_DYNAMIC_STORAGE_BIT);
            newSSBO.get()->m_data = ssbo.get()->m_data;
            newSSBO.get()->m_currentBytes = ssbo.get()->m_currentBytes;

            GLCall(glBindBuffer(GL_COPY_WRITE_BUFFER, newSSBO.get()->glName()));
            GLCall(glBindBuffer(GL_COPY_READ_BUFFER, ssbo.get()->glName()));

            GLCall(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, ssbo->maxSize()));
            ssbo = std::move(newSSBO);
            std::cout << "SSBO resized!\n";
        }

        unsigned int m_indexCount = 0;
        bool m_uploaded = false;
    };
    using RenderUnitSPtr = std::shared_ptr<RenderUnit>;
    using RenderUnitWPtr = std::weak_ptr<RenderUnit>;
}
