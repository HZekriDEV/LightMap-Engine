#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "Application.h"

int main()
{
	LMEditor::Application lmEditorApp{};

	try
	{
		lmEditorApp.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}