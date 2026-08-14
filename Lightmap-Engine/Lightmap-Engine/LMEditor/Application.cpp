#include "Application.h"

namespace LMEditor
{
	void Application::Run()
	{
		while (!m_window.ShouldClose())
		{
			glfwPollEvents();
		}
	}
}