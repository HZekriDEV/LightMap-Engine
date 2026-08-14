#pragma once

#include "Window.h"

namespace LMEditor
{
	class Application
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void Run();

	private:
		LightmapEngine::Window m_window{ WIDTH, HEIGHT, "Lightmap Engine" };

	};
}

