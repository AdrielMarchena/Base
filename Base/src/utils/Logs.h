#pragma once
#include <iostream>

#define DEFAULT_WARNING_TEXT "Warning: "
#define DEBUG_WARNING_TEXT "Warning, Debug action! On function: "
#define NONE_WARNING_TEXT ""


//All confings things
#define LOG_CALL(message,function,file,line)std::cerr << message << function << std::endl <<"\tFile: '" << file << "' <"<< line << ">"<< std::endl << std::endl
#define LOG_NORMAL(message) std::cerr << message << std::endl;
#define LOG_NORMAL_NO_FLUSH(message) std::cerr << message;
#define LOG_WARNING(warning) LOG_CALL(warning,__FUNCTION__,__FILE__,__LINE__)

// Only on DEBUG things
#ifdef _DEBUG
	#define D_LOG(x) std::cout << x << std::endl
	#define DE_LOG(x) std::cerr << x << std::endl
	#define D_ACTION(x) x; LOG_CALL(DEBUG_WARNING_TEXT,__FUNCTION__,__FILE__,__LINE__)
#else
	#define D_LOG(x) 
	#define DE_LOG(x) 
	#define D_ACTION(x) 
#endif
