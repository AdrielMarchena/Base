#include "SandBox.h"

#include "Base/Base.h"
#include "utils/Instrumentor.h"
#include <iostream>
int main()
{
	try
	{
		BASE_PROFILE_BEGIN_SESSION("Starting", "SandBox_Start_Profile.json");
		SandBox* sandbox = new SandBox();
		BASE_PROFILE_END_SESSION();

		sandbox->Run();
		
		BASE_PROFILE_BEGIN_SESSION("Shutdown", "SandBox_Shutdown_Profile.json");
		delete sandbox;
		BASE_PROFILE_END_SESSION();
	}
	catch (const std::exception& ex)
	{
		std::cout << "Unexpected exception: " << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}