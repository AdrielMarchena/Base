/*
*	You probably won't use this file
*
*/

#include <iostream>
#include <exception>
#include "glad/glad.h"
#include "Window.h"

int main()
{
	using namespace Base;
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