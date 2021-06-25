#include <iostream>
#include <exception>
#include "gl/glew.h"
#include "GLFW/glfw3.h"
#include "Game.h"
#include "amb/Ambient.h"

int main()
{
	using namespace en;
	using namespace windowing;
	try
	{
		Game game = Game("Teste");
		game.Run();
		return EXIT_SUCCESS;
	}
	catch (std::exception e)
	{
		std::cout << "Unexpected Error: " << e.what() << std::endl;
	}


	return 0;
}