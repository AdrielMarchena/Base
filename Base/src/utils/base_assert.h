#ifndef BASE_ASSERT
	#define BASE_ASSERT(x) if (!(x)) __debugbreak(); 
#endif