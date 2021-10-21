#include "Maze.h"
#include <iostream>
#include "utils/Instrumentor.h"
int main()
{
	try
	{
		BASE_PROFILE_BEGIN_SESSION("Startup", "Base_StartUp_Profile.json");
		Maze* maze = new Maze();
		BASE_PROFILE_END_SESSION();

		//BASE_PROFILE_BEGIN_SESSION("Runtime", "Base_Runtime_Profile.json");
		maze->Run();
		//BASE_PROFILE_END_SESSION();

		BASE_PROFILE_BEGIN_SESSION("Shutdown","Base_Shutdown_Profile.json");
		delete maze;
		BASE_PROFILE_END_SESSION();

	}
	catch (const std::exception& ex)
	{
		std::cout << "Unexpected exception: " << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}