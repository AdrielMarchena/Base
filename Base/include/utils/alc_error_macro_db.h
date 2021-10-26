#pragma once
#include <iostream>
#include "AL/alc.h"
#include <assert.h>
#include "Log.h"
#ifndef ALC_DEBUG_STUFF
#define ALC_DEBUG_STUFF
#include "base_assert.h"

/* this function can be found here:
    https://stackoverflow.com/questions/18319338/opengl-es-error-to-string
*/
static char const* alc_error_string(ALCenum const err) noexcept
{
    switch (err)
    {
    case ALC_NO_ERROR:
        return "AL_NO_ERROR";
    case ALC_INVALID_ENUM:
        return "AL_INVALID_ENUM";
    case ALC_INVALID_VALUE:
        return "AL_INVALID_VALUE";
    case ALC_OUT_OF_MEMORY:
        return "AL_OUT_OF_MEMORY";
    default:
        BASE_WARN("unknown error");
        return nullptr;
    }
}

/* All things below are from 'The Cherno' youtube Channel */

static void ALCClearError(ALCdevice* device)
{
    if (device == nullptr)
    {
        BASE_ERROR("can't clear or check alc errors, device is nullptr");
        return;
    }
    static ALCenum err;
    uint8_t wd = 255;
    while ((err = alcGetError(device)) != ALC_NO_ERROR && wd--)
    {
    }
}

static bool ALCLogCall(ALCdevice* device,const char* function, const char* file, int line)
{
    if (!device)
    {
        BASE_ERROR("can't clear or check alc errors, device is nullptr");
        return false;
    }
    while (ALCenum error = alcGetError(device))
    {
        BASE_ERROR("[OpenALC error]: {0}, {1}): {2} {3}: {4}", error, alc_error_string(error), function, file, line);
        /*std::cerr << "[OpenAL error] (" << error << ", " << al_error_string(error) << "): " << function <<
            " " << file << ": " << line << std::endl;*/
        return false;
    }
    return true;
}

#if defined(_DEBUG) || defined(DEBUG)
#define ALCCall(x,device) ALCClearError(device);\
     x;\
     BASE_ASSERT(ALCLogCall(device,#x,__FILE__,__LINE__))
#else
#define ALCCall(x) x
#endif
#endif