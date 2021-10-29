#include "Game.h"
#include "utils/Instrumentor.h"
#include "utils/parse_args.h"
#include <iostream>
int main(int argc, char* argv[])
{
	Base::utils::ConsoleArgsParser parser(argc, argv);

	std::string title = parser.GetOption("title");
	float w = parser.GetOptionAsFloat("width");
	float h = parser.GetOptionAsFloat("height");
	bool resizeble = parser.GetFlag("resizeble");

	bool example = parser.GetFlag("example1");

	title = title.empty() ? "Base Engine" : title;
	w = w <= 0 ? 800.0f : w;
	h = h <= 0 ? 600.0f : h;

	try
	{
		BASE_PROFILE_BEGIN_SESSION("Startup", "Life_Startup_Profile.json");
		Game* game = new Game(title.c_str(),w,h,resizeble);
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