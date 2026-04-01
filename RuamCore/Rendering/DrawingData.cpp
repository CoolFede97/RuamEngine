#include "DrawingData.h"

namespace RuamEngine
{

	unsigned int DrawingData::s_instanceIdCount = 0;



	void DrawingData::submitData()
	{
		for (auto& [materialId, renderUnit] : m_renderUnits)
		{
			renderUnit->submitData();
		}
	}

	void DrawingData::submitData(RenderUnit& renderUnit)
	{
		renderUnit.submitData();
	}

	void DrawingData::flush()
	{
		for (auto& [materialId, renderUnit] : m_renderUnits)
		{
			renderUnit->flush();
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
