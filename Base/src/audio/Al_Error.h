#pragma once

#include <string>
#include <exception>
#include "AL/al.h"
#include "AL/alc.h"

#define MY_AL_CHECK_ERROR(check,error,message) \
	if (!check)\
	{\
		if (error = alGetError() != AL_NO_ERROR)\
		{\
			const std::string errorm = message + std::string(", AL ERROR: ") + std::to_string(error);\
			throw std::exception(errorm.c_str());\
		}\
		const std::string errorm = message;\
		throw std::exception(errorm.c_str());\
	}\