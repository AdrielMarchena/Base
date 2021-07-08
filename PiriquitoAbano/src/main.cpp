#include <iostream>
#include "PiriquitoAbano.h"

/*#ifdef _DEBUG

#else // DEBUG

#define _WINDOWS
#undef _CONSOLE
	
#endif // RELEASE*/


//#ifdef _CONSOLE
int main()
{
	try //Try Catch to get any unhandled exceptions
	{
		PiriquitoAbano piriquito;
		//Start the game
		piriquito.Run();
	}
	catch (const std::exception& e)
	{
		std::cout << "Unexpected exeption: " << e.what() << std::endl;
	}
	return 0;
}
//#endif

/*
#ifdef _WINDOWS

int WinMain()
{
	try //Try Catch to get any unhandled exceptions
	{
		PiriquitoAbano piriquito;
		//Start the game
		piriquito.Run();
	}
	catch (const std::exception& e)
	{
		std::cout << "Unexpected exeption: " << e.what() << std::endl;
	}
	return 0;
}

#endif // _WINDOWS
*/
