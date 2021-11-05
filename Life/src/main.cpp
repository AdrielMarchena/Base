#include "Game.h"
#include "utils/Instrumentor.h"
#include "utils/parse_args.h"
#include <iostream>

void DisplayCommands()
{
	std::cout << "--help | -h 'Display this text'" << std::endl;
	std::cout << "--width=positive_number 'window width'" << std::endl;
	std::cout << "--height=positive_number 'window height'" << std::endl;
	std::cout << "--resizeble 'Set window to resizeble or not'" << std::endl;
	std::cout << "--fullscreen 'Open window in full screen'" << std::endl;
	//std::cout << "-----------------------------------------" << std::endl;
	//std::cout << "----------------Advanced-----------------" << std::endl;
	//std::cout << "-----------------------------------------" << std::endl;
	//std::cout << "--framebufferScale=positive_float 'Scale of the framebuffer based on window size,\nThe recomended is a number beetween 0.1 and 1.0'" << std::endl;

	std::cout << "\nThanks for using my program :), have a nice day!" << std::endl;
}

int main(int argc, char* argv[])
{
	float w = 0.0f;
	float h = 0.0f;
	bool resizeble = true;
	bool fullscreen = true;
	std::string title = "";
	bool using_example_instance = false;
	try
	{
		Base::utils::ConsoleArgsParser parser(argc, argv);

		if (parser.GetFlag("help") || parser.GetFlag("h"))
		{
			DisplayCommands();
			return EXIT_SUCCESS;
		}

		title = parser.GetOption("title");
		w = parser.GetOptionAsFloat("width");
		h = parser.GetOptionAsFloat("height");
		resizeble = parser.GetFlag("resizeble");
		fullscreen = parser.GetFlag("fullscreen");

		using_example_instance = parser.GetFlag("example1");
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Could not parse command line args, error: " << ex.what() << std::endl;
		//return EXIT_FAILURE;
	}

	title = title.empty() ? "Base Engine" : title;
	w = w <= 0 ? 800.0f : w;
	h = h <= 0 ? 600.0f : h;

	try
	{
		BASE_PROFILE_BEGIN_SESSION("Startup", "Life_Startup_Profile.json");
		Game* game = new Game(title.c_str(), w, h, resizeble, fullscreen);
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