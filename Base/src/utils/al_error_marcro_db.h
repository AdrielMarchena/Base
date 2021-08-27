#pragma once
#include <iostream>
#include "AL/al.h"
#include <assert.h>
#include "Log.h"
#ifndef AL_DEBUG_STUFF
#define AL_DEBUG_STUFF

/* this function can be found here:
    https://stackoverflow.com/questions/18319338/opengl-es-error-to-string
*/
static char const* al_error_string(ALenum const err) noexcept
{
    switch (err)
    {
    case AL_NO_ERROR:
        return "AL_NO_ERROR";
    case AL_INVALID_ENUM:
        return "AL_INVALID_ENUM";
    case AL_INVALID_VALUE:
        return "AL_INVALID_VALUE";
    case AL_INVALID_OPERATION:
        return "AL_INVALID_OPERATION";
    case AL_OUT_OF_MEMORY:
        return "AL_OUT_OF_MEMORY";
    default:
        BASE_WARN("unknown error");
        return nullptr;
    }
}

/* All things below are from 'The Cherno' youtube Channel */

static void ALClearError()
{
    static ALenum err;
    uint8_t wd = 255;
    while ((err = alGetError()) != AL_NO_ERROR && wd--)
    {
    }
}

static bool ALLogCall(const char* function, const char* file, int line)
{
    while (ALenum error = alGetError())
    {
        BASE_ERROR("[OpenAL error]: {0}, {1}): {2} {3}: {4}", error, al_error_string(error), function, file, line);
        /*std::cerr << "[OpenAL error] (" << error << ", " << al_error_string(error) << "): " << function <<
            " " << file << ": " << line << std::endl;*/
        return false;
    }
    return true;
}

#define BASE_ASSERT(x) if (!(x)) __debugbreak(); 

#if defined(_DEBUG) || defined(DEBUG)
#define ALCall(x) ALClearError();\
     x;\
     BASE_ASSERT(ALLogCall(#x,__FILE__,__LINE__))
#else
#define ALCall(x) x
#endif
#endif