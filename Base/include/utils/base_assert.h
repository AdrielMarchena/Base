#include "Log.h"
#include "debugbreak.h"
#if defined(BASE_DEBUG) //|| defined(BASE_RELEASE)
	#ifndef BASE_ASSERT
		#define BASE_ASSERT(x) if (!(x)) debug_break(); 
	#endif
	#ifndef BASE_CORE_ASSERT
		#define BASE_CORE_ASSERT(x,...) if (!(x)){ BASE_ERROR(__VA_ARGS__); debug_break(); } 
	#endif

#else 

	#ifndef BASE_ASSERT
		#define BASE_ASSERT(x)
	#endif

	#ifndef BASE_CORE_ASSERT
		#define BASE_CORE_ASSERT(x,message)
	#endif

#endif //defined(BASE_DEBUG) || defined(BASE_RELEASE)