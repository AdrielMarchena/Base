#include "Log.h"
#if defined(_DEBUG)
	#ifndef BASE_ASSERT
		#define BASE_ASSERT(x) if (!(x)) __debugbreak(); 
	#endif
	#ifndef BASE_CORE_ASSERT
		#define BASE_CORE_ASSERT(x,message) if (!(x)){ BASE_ERROR(message); __debugbreak(); } 
	#endif

#else //defined(_DEBUG)

	#ifndef BASE_ASSERT
		#define BASE_ASSERT(x)
	#endif

	#ifndef BASE_CORE_ASSERT
		#define BASE_CORE_ASSERT(x,message)
	#endif

#endif //defined(_DEBUG)