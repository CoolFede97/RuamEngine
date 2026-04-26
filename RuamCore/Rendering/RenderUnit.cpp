#include "RenderUnit.h"

namespace RuamEngine
{
	void RenderUnit::submitData()
	{
		if (m_vertices->currentSize() > 0)
		{
			if (!m_staticStorage || !m_uploaded)
			{
				m_vertices->submitData();
			}
		}
		if (m_indices->currentSize() > 0)
		{
			if (!m_staticStorage || !m_uploaded)
			{
				m_indices->submitData();
			}
		}
		if (m_modelMatrices->currentSize() > 0)
		{
			m_modelMatrices->submitData();
		}

		if (m_staticStorage && !m_uploaded && (m_vertices->currentSize() > 0 || m_indices->currentSize() > 0))
		{
			m_uploaded = true;
		}
	}

	void RenderUnit::bindBuffersBase()
	{
		m_vertices->bindBufferBase(SSBOType::vertices);
		m_indices->bindBufferBase(SSBOType::indices);
		m_modelMatrices->bindBufferBase(SSBOType::modelMatrices);
	}

	/*bool RenderUnit::pushBatchData(const std::vector<Vertex> vertices, unsigned int vertexDataSize, const std::vector<unsigned int> indices, unsigned int indexDataSize)
	{
		bool fullBatch = false;
		ASSERT(indexDataSize <= m_indexBuffer->GetMaxSize());

		if (m_indexBuffer->GetCurrentSize() + indexDataSize > m_indexBuffer->GetMaxSize())
		{
			SubmitData();
			Renderer::Draw(*this);
			Flush();
			fullBatch = true;
		}
		m_vertices->pushBatchData(vertices);
		m_indices->pushBatchData(indices);
		return fullBatch;
	}*/

	void RenderUnit::pushBatchData(const std::vector<Vertex>& vertices, std::vector<unsigned int> indices, const std::vector<glm::mat4>& modelMatrices)
	{
		if (m_vertices->checkIfPushIsBiggerThanMaxSize(vertices.size())) resizeSSBO(m_vertices, true);
		if (m_indices->checkIfPushIsBiggerThanMaxSize(indices.size())) resizeSSBO(m_indices, true);
		if (m_modelMatrices->checkIfPushIsBiggerThanMaxSize(modelMatrices.size())) resizeSSBO(m_modelMatrices, true);

		for (unsigned int i = 0; i < indices.size() ; i++)
		{
			indices[i] += m_indexCount;
		}
		m_indexCount += vertices.size();

		if (!m_vertices->checkIfEnoughSpaceForPush(vertices.size())) resizeSSBO(m_vertices, false);
		m_vertices->pushBatchData(vertices);

		if (!m_indices->checkIfEnoughSpaceForPush(indices.size())) resizeSSBO(m_indices, false);
        m_indices->pushBatchData(indices);

        if (!m_modelMatrices->checkIfEnoughSpaceForPush(modelMatrices.size())) resizeSSBO(m_modelMatrices, false);
		m_modelMatrices->pushBatchData(modelMatrices);
	}

	void RenderUnit::pushModelMatrices(const std::vector<glm::mat4>& modelMatrices)
	{
		std::cout << "Fullness: " << m_modelMatrices->currentSize()+modelMatrices.size() * mat4Size << " / " << m_modelMatrices->maxSize() << "\n";
		ASSERT(modelMatrices.size() * mat4Size <= m_modelMatrices->maxSize());
		bool fullBatch = false;
		if (!m_modelMatrices->checkIfEnoughSpaceForPush(modelMatrices.size())) resizeSSBO(m_modelMatrices, false);
	    m_modelMatrices->pushBatchData(modelMatrices);
	}

	void RenderUnit::flush()
	{
		if (!m_staticPosition) m_modelMatrices->flush();
		if (!m_staticStorage)
		{
			m_indexCount = 0;
			m_vertices->flush();
			m_indices->flush();
		}
	}

	RenderUnit::RenderUnit()
	{

	}
}
