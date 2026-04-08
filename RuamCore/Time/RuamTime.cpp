#include "RuamTime.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace RuamEngine
{

	float RuamTime::m_currentFrame = 0.0f;
	float RuamTime::m_lastFrame = 0.0f;

	void RuamTime::Update()
	{
		m_lastFrame = m_currentFrame;
		m_currentFrame = glfwGetTime();
	}
}
