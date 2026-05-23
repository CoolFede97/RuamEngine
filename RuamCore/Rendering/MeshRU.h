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

        MeshRU();

        MaterialWPtr m_material = {};
        VertexArrayUPtr m_vertexArray = std::make_unique<VertexArray>();
        SSBOUPtr<Vertex> m_vertices = std::make_unique<SSBO<Vertex>>(maxVertexCount, GL_DYNAMIC_STORAGE_BIT);
        SSBOUPtr<unsigned int> m_indices = std::make_unique<SSBO<unsigned int>>(maxIndexCount, GL_DYNAMIC_STORAGE_BIT);

        void submitData();
		void bindBuffersBase();

        void pushBatchData(const std::vector<Vertex>& vertices, std::vector<unsigned int> indices, const std::vector<glm::mat4>& modelMatrices);
    private:
        template<typename T>
        void resizeSSBO(SSBOUPtr<T>& ssbo, bool pushExceedsCapacity, unsigned int elementsToSupport)
        {
            unsigned int amplifier = pushExceedsCapacity ? ssboOversizeMultiplier : ssboAmplifier;
            for (unsigned int amplifyTry = 1; amplifyTry <=amplificationTries; amplifyTry++)
            {
                if ((ssbo->maxSize())*amplifier*amplifyTry<elementsToSupport) continue;
                SSBOUPtr<T> newSSBO = std::make_unique<SSBO<T>>(ssbo->maxElements()*amplifier*amplifyTry, GL_DYNAMIC_STORAGE_BIT);
                newSSBO.get()->m_data = ssbo.get()->m_data;
                newSSBO.get()->m_currentBytes = ssbo.get()->m_currentBytes;

                GLCall(glBindBuffer(GL_COPY_WRITE_BUFFER, newSSBO.get()->glName()));
                GLCall(glBindBuffer(GL_COPY_READ_BUFFER, ssbo.get()->glName()));

                GLCall(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, ssbo->maxSize()));
                ssbo = std::move(newSSBO);
                std::cout << "SSBO resized!\n";
                return;
            }
            std::cerr << "SSBO was resized " << amplificationTries << " times and still it can't support the elements requiered: " << elementsToSupport << " bytes\n";
            return;
        }

        unsigned int m_indexCount = 0;
        bool m_uploaded = false;
    };
    using MeshRUSPtr = std::shared_ptr<MeshRU>;
    using MeshRUWPtr = std::weak_ptr<MeshRU>;
}
