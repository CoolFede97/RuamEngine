#include "RenderUnit.h"
#include "Renderer.h"

namespace RuamEngine
{
	void RenderUnit::SubmitBatchData()
	{
		if (m_vertices->GetCurrentSize() > 0)
		{
			m_vertices->SubmitData();
			m_vertices->BindBufferBase(SSBOType::vertices);
		}
		if (m_indices->GetCurrentSize() > 0)
		{
			m_indices->SubmitData();
			m_indices->BindBufferBase(SSBOType::indices);
		}
		if (m_modelMatricesBuffer->GetCurrentSize() > 0)
		{
			m_modelMatricesBuffer->SubmitData();
			m_modelMatricesBuffer->BindBufferBase(SSBOType::modelMatrices);
		}
	}

	/*bool RenderUnit::AddBatchData(const std::vector<Vertex> vertices, unsigned int vertexDataSize, const std::vector<unsigned int> indices, unsigned int indexDataSize)
	{
		bool fullBatch = false;
		ASSERT(indexDataSize <= m_indexBuffer->GetMaxSize());

		if (m_indexBuffer->GetCurrentSize() + indexDataSize > m_indexBuffer->GetMaxSize())
		{
			SubmitBatchData();
			Renderer::Draw(*this);
			Flush();
			fullBatch = true;
		}
		m_vertices->AddBatchData(vertices);
		m_indices->AddBatchData(indices);
		return fullBatch;
	}*/

	bool RenderUnit::AddBatchData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<glm::mat4>& modelMatrices)
	{
		ASSERT(vertices.size() * sizeof(Vertex) <= m_vertices->GetMaxSize());
		ASSERT(indices.size() * sizeof(unsigned int) <= m_indices->GetMaxSize());
		ASSERT(modelMatrices.size() * mat4Size <= m_modelMatricesBuffer->GetMaxSize());

		bool fullBatch = false;

		if (m_vertices->GetCurrentSize() + vertices.size() * sizeof(float) > m_vertices->GetMaxSize())
		{
			SubmitBatchData();
			Renderer::Draw(*this);
			Flush();
			fullBatch = true;
		}
		m_vertices->AddBatchData(vertices);
		m_indices->AddBatchData(indices);
		m_modelMatricesBuffer->AddBatchData(modelMatrices);
		return fullBatch;
	}

	void RenderUnit::Flush()
	{
		m_vertices->Flush();
		m_indices->Flush();
		m_modelMatricesBuffer->Flush();
	}

	RenderUnit::RenderUnit(ShaderPtr& shader)
		: m_shader(shader)
	{

	}
	RenderUnit::RenderUnit()
	{

	}
}