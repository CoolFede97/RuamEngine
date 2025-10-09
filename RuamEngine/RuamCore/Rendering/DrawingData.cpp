#include "DrawingData.h"

namespace RuamEngine
{

	void DrawingData::SubmitBatchData()
	{
		for (auto& pair : m_renderUnits)
		{
			pair.second.SubmitBatchData();
		}
	}

	void DrawingData::SubmitBatchData(RenderUnit& renderUnit)
	{
		renderUnit.SubmitBatchData();
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


