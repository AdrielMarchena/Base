#include "SandBox.h"

int main()
{
	try
	{
		SandBox* sandbox = new SandBox();
		sandbox->Run();
		delete sandbox;
	}
	catch (const std::exception& ex)
	{
		std::cout << "Unexpected exception: " << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}