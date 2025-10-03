#include "DrawingData.h"

namespace RuamEngine
{

	void DrawingData::SubmitBatchData()
	{
		for (auto& pair : m_renderUnits)
		{
			if (pair.second.m_vertexBuffer->GetCurrentSize() > 0)
			{
				pair.second.m_vertexBuffer->SubmitData();
			}
			if (pair.second.m_indexBuffer->GetCurrentSize() > 0)
			{
				pair.second.m_indexBuffer->SubmitData();
			}
		}
	}

	void DrawingData::SubmitBatchData(RenderUnit& renderUnit)
	{
		if (renderUnit.m_vertexBuffer->GetCurrentSize() > 0)
		{
			renderUnit.m_vertexBuffer->SubmitData();
		}
		if (renderUnit.m_indexBuffer->GetCurrentSize() > 0)
		{
			renderUnit.m_indexBuffer->SubmitData();
		}	
	}

	void DrawingData::Flush()
	{
		for (auto& pair : m_renderUnits)
		{
			pair.second.Flush();
		}
	}

	DrawingData::DrawingData(Shader::PipelineType pipelineType)
		: m_pipelineType(pipelineType)
	{
	}

	DrawingData::DrawingData()
	{
	}
	DrawingData::~DrawingData()
	{
		std::cout << "Drawing Data Destroyed!\n";
	}

}


