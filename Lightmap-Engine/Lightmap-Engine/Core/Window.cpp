#include "Window.h"

namespace LightmapEngine
{
	Window::Window(int width, int height, std::string name) : m_width(width), m_height(height)
	{
		InitWindow(width, height, name);
	}

	Window::~Window()
	{
		if (m_window)
		{
			glfwDestroyWindow(m_window);
			glfwTerminate();
		}
	}

	void Window::InitWindow(int width, int height, std::string name)
	{
		glfwInit();

		// Don't create an OpenGL context
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		// Don't allow resize for now
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	}
}