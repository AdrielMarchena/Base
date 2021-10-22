#include "Example1.h"
#include "SandBox.h"
#include "Base/Base.h"
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
		BASE_PROFILE_BEGIN_SESSION("Starting", "SandBox_Start_Profile.json");
		Base::windowing::Window* app = nullptr;
		if(example)
			app = new Example1(title.c_str(),w,h,resizeble);
		else
			app = new SandBox(title.c_str(), w, h, resizeble);
		BASE_PROFILE_END_SESSION();

		app->Run();
		
		BASE_PROFILE_BEGIN_SESSION("Shutdown", "SandBox_Shutdown_Profile.json");
		delete app;
		BASE_PROFILE_END_SESSION();
	}
	catch (const std::exception& ex)
	{
		std::cout << "Unexpected exception: " << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}