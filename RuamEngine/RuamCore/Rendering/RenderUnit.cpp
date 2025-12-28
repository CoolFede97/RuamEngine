#include "RenderUnit.h"
#include "Renderer.h"

namespace RuamEngine
{
	void RenderUnit::SubmitData()
	{
		if (m_vertices->GetCurrentSize() > 0)
		{
			if (!m_staticStorage || !m_uploaded)
			{
				m_vertices->SubmitData();
			}
		}
		if (m_indices->GetCurrentSize() > 0)
		{
			if (!m_staticStorage || !m_uploaded)
			{
				m_indices->SubmitData();
			}
		}
		if (m_modelMatricesBuffer->GetCurrentSize() > 0)
		{
			m_modelMatricesBuffer->SubmitData();
		}

		if (m_staticStorage && !m_uploaded && (m_vertices->GetCurrentSize() > 0 || m_indices->GetCurrentSize() > 0))
		{
			m_uploaded = true;
		}
	}

	void RenderUnit::BindBuffersBase()
	{
		m_vertices->BindBufferBase(SSBOType::vertices);
		m_indices->BindBufferBase(SSBOType::indices);
		m_modelMatricesBuffer->BindBufferBase(SSBOType::modelMatrices);
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

	bool RenderUnit::AddBatchData(const std::vector<Vertex>& vertices, std::vector<unsigned int> indices, const std::vector<glm::mat4>& modelMatrices)
	{
		ASSERT(vertices.size() * sizeof(Vertex) <= m_vertices->GetMaxSize());
		ASSERT(indices.size() * sizeof(unsigned int) <= m_indices->GetMaxSize());
		ASSERT(modelMatrices.size() * mat4Size <= m_modelMatricesBuffer->GetMaxSize());

		for (unsigned int i = 0; i < indices.size() ; i++)
		{
			indices[i] += m_indexCount;
		}
		m_indexCount += indices.size();
		bool fullBatch = false;

		if (m_vertices->GetCurrentSize() + vertices.size() * sizeof(Vertex) > m_vertices->GetMaxSize())
		{
			SubmitData();
			BindBuffersBase();
			Renderer::Draw(*this);
			Flush();
			fullBatch = true;
		}

		m_vertices->AddBatchData(vertices);
		m_indices->AddBatchData(indices);
		m_modelMatricesBuffer->AddBatchData(modelMatrices);
		return fullBatch;
	}

	bool RenderUnit::AddModelMatrix(const std::vector<glm::mat4>& modelMatrices)
	{
		ASSERT(modelMatrices.size() * mat4Size <= m_modelMatricesBuffer->GetMaxSize());
		bool fullBatch = false;
		if (m_modelMatricesBuffer->GetCurrentSize() + modelMatrices.size() * mat4Size > m_modelMatricesBuffer->GetMaxSize())
		{
			SubmitData();
			Renderer::Draw(*this);
			Flush();
			fullBatch = true;
		}
		m_modelMatricesBuffer->AddBatchData(modelMatrices);
		return fullBatch;
	}

	void RenderUnit::Flush()
	{
		if (!m_staticPosition) m_modelMatricesBuffer->Flush();
		if (!m_staticStorage)
		{
			m_indexCount = 0;
			m_vertices->Flush();
			m_indices->Flush();
		}
	}

	RenderUnit::RenderUnit()
	{

	}
}
