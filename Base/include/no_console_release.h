#pragma once

#ifdef NO_CONSOLE_RELEASE 

#ifdef _DEBUG

#pragma comment(linker,"/subsystem:console")

#else // _DEBUG

#pragma comment(linker,"/subsystem:windows")

#endif // DEBUG

#else // NO_CONSOLE_RELEASE

#pragma comment(linker,"/subsystem:console")

#endif