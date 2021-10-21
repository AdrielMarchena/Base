#pragma once

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
namespace Base
{
class Log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
private:
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_ClientLogger;
};
}

#if defined BASE_DEBUG || defined BASE_RELEASE //Debug / Release defs

//Core Errors macros
/* Macro for loggin engine error's */
#define BASE_ERROR(...) ::Base::Log::GetCoreLogger()->error(__VA_ARGS__)
/* Macro for loggin engine warning's */
#define BASE_WARN(...)  ::Base::Log::GetCoreLogger()->warn(__VA_ARGS__)
/* Macro for loggin engine info's */
#define BASE_INFO(...)  ::Base::Log::GetCoreLogger()->info(__VA_ARGS__)
/* Macro for loggin engine debug info */
#define BASE_DEBUG_LOG(...) ::Base::Log::GetCoreLogger()->debug(__VA_ARGS__)
/* Macro for loggin engine trace */
#define BASE_TRACE(...) ::Base::Log::GetCoreLogger()->trace(__VA_ARGS__)
/* Macro for loggin engine fatal(errors probably) */
#define BASE_FATAL(...) ::Base::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Application Errors macros
/* Macro for loggin application/game error's */
#define APP_ERROR(...) ::Base::Log::GetClientLogger()->error(__VA_ARGS__)
/* Macro for loggin application/game wanning's */
#define APP_WARN(...)  ::Base::Log::GetClientLogger()->warn(__VA_ARGS__)
/* Macro for loggin application/game info's */
#define APP_INFO(...)  ::Base::Log::GetClientLogger()->info(__VA_ARGS__)
/* Macro for loggin application/game debug info */
#define APP_DEBUG(...) ::Base::Log::GetClientLogger()->debug(__VA_ARGS__)
/* Macro for loggin application/game trace */
#define APP_TRACE(...) ::Base::Log::GetClientLogger()->trace(__VA_ARGS__)
/* Macro for loggin application/game fatal(errors probably) */
#define APP_FATAL(...) ::Base::Log::GetClientLogger()->fatal(__VA_ARGS__)

#else //Dist defs

//Core Errors macros
/* Macro for loggin engine error's */
#define BASE_ERROR(...) 
/* Macro for loggin engine warning's */
#define BASE_WARN(...) 
/* Macro for loggin engine info's */
#define BASE_INFO(...) 
/* Macro for loggin engine debug info */
#define BASE_DEBUG_LOG(...) 
/* Macro for loggin engine trace */
#define BASE_TRACE(...) 
/* Macro for loggin engine fatal(errors probably) */
#define BASE_FATAL(...) 

//Application Errors macros
/* Macro for loggin application/game error's */
#define APP_ERROR(...) 
/* Macro for loggin application/game wanning's */
#define APP_WARN(...) 
/* Macro for loggin application/game info's */
#define APP_INFO(...) 
/* Macro for loggin application/game debug info */
#define APP_DEBUG(...) 
/* Macro for loggin application/game trace */
#define APP_TRACE(...) 
/* Macro for loggin application/game fatal(errors probably) */
#define APP_FATAL(...) 

#endif
