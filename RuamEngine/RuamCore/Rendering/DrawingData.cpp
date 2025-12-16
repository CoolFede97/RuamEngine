#include "DrawingData.h"

namespace RuamEngine
{

	unsigned int DrawingData::s_instanceIdCount = 0;



	void DrawingData::SubmitData()
	{
		for (RenderUnitPtr renderUnit : m_renderUnits)
		{
			renderUnit->SubmitData();
		}
	}

	void DrawingData::SubmitData(RenderUnit& renderUnit)
	{
		renderUnit.SubmitData();
	}

	void DrawingData::Flush()
	{
		for (RenderUnitPtr renderUnit: m_renderUnits)
		{
			renderUnit->Flush();
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
