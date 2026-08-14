#pragma once

#include <GLFW/glfw3.h>
#include <string>

namespace LightmapEngine
{
	class Window
	{
	public:
		Window(int width, int height, std::string name);
		~Window();
		void InitWindow(int width, int height, std::string name);
		inline bool ShouldClose() { return glfwWindowShouldClose(m_window); };

	private:
		GLFWwindow* m_window;
		const int m_width;
		const int m_height;
	};
}