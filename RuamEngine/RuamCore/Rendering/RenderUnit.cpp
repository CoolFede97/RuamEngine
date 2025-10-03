#include "RenderUnit.h"
#include "Renderer.h"

namespace RuamEngine
{
	void RenderUnit::SubmitBatchData()
	{
		if (m_vertexBuffer->GetCurrentSize() > 0)
		{
			m_vertexBuffer->SubmitData();
		}
		if (m_indexBuffer->GetCurrentSize() > 0)
		{
			m_indexBuffer->SubmitData();
		}
	}

	bool RenderUnit::AddBatchData(const std::vector<float> vertices, unsigned int vertexDataSize, const std::vector<unsigned int> indices, unsigned int indexDataSize)
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
		m_vertexBuffer->AddBatchData(vertices, vertexDataSize);
		m_indexBuffer->AddBatchData(indices, indexDataSize);
		return fullBatch;
	}

	void RenderUnit::Flush()
	{
		m_vertexBuffer->Flush();
		m_indexBuffer->Flush();
	}

	RenderUnit::RenderUnit(ShaderPtr& shader)
		: m_shader(shader)
	{

	}
	RenderUnit::RenderUnit()
	{

	}
}