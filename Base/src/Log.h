#pragma once

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
namespace en
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

//Core Errors macros
/* Macro for loggin engine error's */
#define BASE_ERROR(...) ::en::Log::GetCoreLogger()->error(__VA_ARGS__)
/* Macro for loggin engine warning's */
#define BASE_WARN(...)  ::en::Log::GetCoreLogger()->warn(__VA_ARGS__)
/* Macro for loggin engine info's */
#define BASE_INFO(...)  ::en::Log::GetCoreLogger()->info(__VA_ARGS__)
#if defined(_DEBUG)
	/* Macro for loggin engine debug info */
	#define BASE_DEBUG(...) ::en::Log::GetCoreLogger()->debug(__VA_ARGS__)
#else
	#define BASE_DEBUG(...) 
#endif
/* Macro for loggin engine debug info (appers in release)*/
#define BASE_RDEBUG(...) ::en::Log::GetCoreLogger()->debug(__VA_ARGS__)
/* Macro for loggin engine trace */
#define BASE_TRACE(...) ::en::Log::GetCoreLogger()->trace(__VA_ARGS__)
/* Macro for loggin engine fatal(errors probably) */
#define BASE_FATAL(...) ::en::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Application Errors macros

/* Macro for loggin application/game error's */
#define APP_ERROR(...) ::en::Log::GetClientLogger()->error(__VA_ARGS__)
/* Macro for loggin application/game wanning's */
#define APP_WARN(...)  ::en::Log::GetClientLogger()->warn(__VA_ARGS__)
/* Macro for loggin application/game info's */
#define APP_INFO(...)  ::en::Log::GetClientLogger()->info(__VA_ARGS__)
#if defined(_DEBUG)
	/* Macro for loggin application/game debug info */
	#define APP_DEBUG(...) ::en::Log::GetClientLogger()->debug(__VA_ARGS__)
#else
	#define APP_DEBUG(...)
#endif
/* Macro for loggin application/game debug info (appers in release)*/
#define APP_RDEBUG(...) ::en::Log::GetCoreLogger()->debug(__VA_ARGS__)
/* Macro for loggin application/game trace */
#define APP_TRACE(...) ::en::Log::GetClientLogger()->trace(__VA_ARGS__)
/* Macro for loggin application/game fatal(errors probably) */
#define APP_FATAL(...) ::en::Log::GetClientLogger()->fatal(__VA_ARGS__)