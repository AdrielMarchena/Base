#include <iostream>
#include "PiriquitoAbano.h"

//#define NO_CONSOLE_RELEASE
#include "no_console_release.h"

int main()
{
	PiriquitoAbano* piriquito = nullptr;
	try //Try Catch to get any unhandled exceptions
	{
		piriquito = new PiriquitoAbano();
		//Start the game
		piriquito->Run();
		delete piriquito;
	}
	catch (const std::exception& e)
	{
		//TODO: Use Log lib for log this latter
		std::cout << "Unexpected exeption: " << e.what() << std::endl;
		if (piriquito)
			delete piriquito;
	}
	return 0;
}
/*
int WinMain()
{
	PiriquitoAbano* piriquito = nullptr;
	try //Try Catch to get any unhandled exceptions
	{
		piriquito = new PiriquitoAbano();
		//Start the game
		piriquito->Run();
		delete piriquito;
	}
	catch (const std::exception& e)
	{
		//TODO: Use Log lib for log this latter
		std::cout << "Unexpected exeption: " << e.what() << std::endl;
		if (piriquito)
			delete piriquito;
	}
}
*/