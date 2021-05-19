#include <iostream>
#include <exception>
#include "gl/glew.h"
#include "Window.h"

int main()
{
	using namespace en;
	using namespace windowing;
	try
	{
		Window window = Window("Teste");
		window.Run();
		return EXIT_SUCCESS;
	}
	catch (std::exception e)
	{
		std::cout << "Unexpected Error: " << e.what() << std::endl;
	}
	return 0;
}