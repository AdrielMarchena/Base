#include "Game.h"
#include "utils/Instrumentor.h"
#include <iostream>
int main()
{
	try
	{
		BASE_PROFILE_BEGIN_SESSION("Startup", "Life_Startup_Profile.json");
		Game* game = new Game();
		BASE_PROFILE_END_SESSION();

		game->Run();

		BASE_PROFILE_BEGIN_SESSION("Shutdown", "Life_Shutdown_Profile.json");
		delete game;
		BASE_PROFILE_END_SESSION();

		return EXIT_SUCCESS;
	}
	catch (std::exception e)
	{
		std::cout << "Unexpected Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}