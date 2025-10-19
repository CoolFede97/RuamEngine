#include "DrawingData.h"

namespace RuamEngine
{

	unsigned int DrawingData::s_instanceIdCount = 0;

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

	DrawingData::DrawingData()
		: m_instanceId(s_instanceIdCount++)
	{
	}
	DrawingData::~DrawingData()
	{
		std::cout << "Drawing Data Destroyed!\n";
	}

}


