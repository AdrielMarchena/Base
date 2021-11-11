#include "SandBox.h"
#include "Base/Base.h"
#include "utils/Instrumentor.h"
#include "utils/parse_args.h"
#include <iostream>

void DisplayCommands()
{
	std::cout << "-----All options and flags are registred in lower case-----\n" << std::endl;
	std::cout << "--help | -h 'Display this text'" << std::endl;
	std::cout << "--width=positive_number 'window width'" << std::endl;
	std::cout << "--height=positive_number 'window height'" << std::endl;
	std::cout << "--resizeble 'Set window to resizeble or not'" << std::endl;
	std::cout << "--fullscreen 'Open window in full screen'" << std::endl;
	std::cout << "--vSync-off 'Disable VSync'" << std::endl;
	std::cout << "--titleOff 'Disable title bar even if fullscreen is not on'" << std::endl;
	//std::cout << "-----------------------------------------" << std::endl;
	//std::cout << "----------------Advanced-----------------" << std::endl;
	//std::cout << "-----------------------------------------" << std::endl;
	//std::cout << "--framebufferScale=positive_float 'Scale of the framebuffer based on window size,\nThe recomended is a number beetween 0.1 and 1.0'" << std::endl;

	std::cout << "\nThanks for using my program :), have a nice day!" << std::endl;
}

int main(int argc, char* argv[])
{
	Base::windowing::WindowSpecification specifications;

	specifications.width = 0.0f;
	specifications.height = 0.0f;
	specifications.resizeble = true;
	specifications.fullscreen = true;
	specifications.title = "";
	specifications.title_bar_off = false;
	specifications.v_sync_on = true;
	bool using_example_instance = false;
	try
	{
		Base::utils::ConsoleArgsParser parser(argc, argv);

		if (parser.GetFlag("help") || parser.GetFlag("h"))
		{
			DisplayCommands();
			return EXIT_SUCCESS;
		}

		specifications.title = parser.GetOption("title");
		specifications.width = parser.GetOptionAsInt("width");
		specifications.height = parser.GetOptionAsInt("height");
		specifications.resizeble = parser.GetFlag("resizeble");
		specifications.fullscreen = parser.GetFlag("fullscreen");
		specifications.title_bar_off = parser.GetFlag("titleoff");
		specifications.v_sync_on = parser.GetFlag("vSync-off");
		using_example_instance = parser.GetFlag("example1");
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Could not parse command line args, error: " << ex.what() << std::endl;
		//return EXIT_FAILURE;
	}

	specifications.title = specifications.title.empty() ? "Base Engine" : specifications.title;
	specifications.width = specifications.width < 1 ? 800.0f : specifications.width;
	specifications.height = specifications.height < 1 ? 600.0f : specifications.height;

	try
	{
		BASE_PROFILE_BEGIN_SESSION("Starting", "SandBox_Start_Profile.json");
		Base::windowing::Window* app = nullptr;
		if(using_example_instance)
			app = new SandBox(specifications);
		else
			app = new SandBox(specifications);
		BASE_PROFILE_END_SESSION();

		app->Run();
		
		BASE_PROFILE_BEGIN_SESSION("Shutdown", "SandBox_Shutdown_Profile.json");
		delete app;
		BASE_PROFILE_END_SESSION();
	}
	catch (const Base::utils::baseException::WindowCreationException& ex)
	{
		std::cout << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (const std::exception& ex)
	{
		std::cout << "Unexpected exception: " << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}