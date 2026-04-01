#include "RenderUnit.h"
#include "Renderer.h"

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
		if (m_modelMatricesBuffer->currentSize() > 0)
		{
			m_modelMatricesBuffer->submitData();
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
		m_modelMatricesBuffer->bindBufferBase(SSBOType::modelMatrices);
	}

	/*bool RenderUnit::AddBatchData(const std::vector<Vertex> vertices, unsigned int vertexDataSize, const std::vector<unsigned int> indices, unsigned int indexDataSize)
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
		m_vertices->AddBatchData(vertices);
		m_indices->AddBatchData(indices);
		return fullBatch;
	}*/

	bool RenderUnit::addBatchData(const std::vector<Vertex>& vertices, std::vector<unsigned int> indices, const std::vector<glm::mat4>& modelMatrices)
	{
		ASSERT(vertices.size() * sizeof(Vertex) <= m_vertices->maxSize());
		ASSERT(indices.size() * sizeof(unsigned int) <= m_indices->maxSize());
		ASSERT(modelMatrices.size() * mat4Size <= m_modelMatricesBuffer->maxSize());

		for (unsigned int i = 0; i < indices.size() ; i++)
		{
			indices[i] += m_indexCount;
		}
		m_indexCount += vertices.size();
		bool fullBatch = false;

		if (m_vertices->currentSize() + vertices.size() * sizeof(Vertex) > m_vertices->maxSize())
		{
			submitData();
			bindBuffersBase();
			Renderer::Draw(*this);
			flush();
			fullBatch = true;
		}

		m_vertices->addBatchData(vertices);
		m_indices->addBatchData(indices);
		m_modelMatricesBuffer->addBatchData(modelMatrices);
		return fullBatch;
	}

	bool RenderUnit::addModelMatrix(const std::vector<glm::mat4>& modelMatrices)
	{
		ASSERT(modelMatrices.size() * mat4Size <= m_modelMatricesBuffer->maxSize());
		bool fullBatch = false;
		if (m_modelMatricesBuffer->currentSize() + modelMatrices.size() * mat4Size > m_modelMatricesBuffer->maxSize())
		{
			submitData();
			Renderer::Draw(*this);
			flush();
			fullBatch = true;
		}
		m_modelMatricesBuffer->addBatchData(modelMatrices);
		return fullBatch;
	}

	void RenderUnit::flush()
	{
		if (!m_staticPosition) m_modelMatricesBuffer->flush();
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
