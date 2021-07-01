#include <iostream>
#include "PiriquitoAbano.h"
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