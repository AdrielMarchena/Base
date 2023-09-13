#pragma once

#include "Log.h"
#include "debugbreak.h"
#include <string>
#ifdef BASE_DEBUG
#include <typeinfo>
#define BASE_TYPEID_NAME(type) typeid(type).name()
#endif

static std::string BaseParseTypeIDName(const std::string& name)
{
	std::string nm = name;
	auto dot_dot_loc = nm.find_last_of("::");

	if (dot_dot_loc != std::string::npos)
		return nm.substr(dot_dot_loc + 1);

	return std::string();
}

#if defined(BASE_DEBUG)
#ifndef BASE_DEBUG_BREAK()
#define BASE_DEBUG_BREAK() debug_break()
#endif
#else
#ifndef BASE_DEBUG_BREAK()
#define BASE_DEBUG_BREAK() 
#endif
#endif

#if defined(BASE_DEBUG) //|| defined(BASE_RELEASE)
#ifndef BASE_ASSERT
#define BASE_ASSERT(x) if (!(x)) BASE_DEBUG_BREAK(); 
#endif
#ifndef BASE_CORE_ASSERT
#define BASE_CORE_ASSERT(x,...) if (!(x)){ BASE_ERROR(__VA_ARGS__); BASE_DEBUG_BREAK(); } 
#endif
#ifndef BASE_PARSE_TYPE_NAME
#define BASE_PARSE_TYPE_NAME(name) BaseParseTypeIDName(name)
#endif
#ifndef BASE_GET_PARSE_TYPE_NAME
#define BASE_GET_PARSE_TYPE_NAME(type) BASE_PARSE_TYPE_NAME(BASE_TYPEID_NAME(type))
#endif
#else 
#define BASE_TYPEID_NAME(type)

#ifndef BASE_ASSERT
#define BASE_ASSERT(x)
#endif

#ifndef BASE_CORE_ASSERT
#define BASE_CORE_ASSERT(x,...)
#endif
#ifndef BASE_PARSE_TYPE_NAME
#define BASE_PARSE_TYPE_NAME(name)
#endif
#ifndef BASE_GET_PARSE_TYPE_NAME
#define BASE_GET_PARSE_TYPE_NAME(type)
#endif
#endif //defined(BASE_DEBUG) || defined(BASE_RELEASE)