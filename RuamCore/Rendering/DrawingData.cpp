#include "DrawingData.h"

namespace RuamEngine
{
	unsigned int DrawingData::s_instanceIdCount = 0;

	DrawingData::DrawingData()
		: m_instanceId(s_instanceIdCount++)
	{
	}
	DrawingData::~DrawingData()
	{
		std::cout << "Drawing Data Destroyed!\n";
	}

}
