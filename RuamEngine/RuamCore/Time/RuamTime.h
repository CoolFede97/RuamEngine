#pragma once

#include "GLFW/glfw3.h"

namespace RuamEngine
{
	class Time
	{
	private:
		static float m_currentFrame;
		static float m_lastFrame;
	public:
		static void Update();
		static float DeltaTime() { return m_currentFrame - m_lastFrame; }
	};
}

