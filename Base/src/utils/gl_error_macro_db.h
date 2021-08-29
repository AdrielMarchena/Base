#pragma once
#include <iostream>
#include <gl/glew.h>
#include <assert.h>
#include "Log.h"

#ifndef GL_DEBUG_STUFF
#define GL_DEBUG_STUFF

#include "base_assert.h"
/* this function can be found here:
    https://stackoverflow.com/questions/18319338/opengl-es-error-to-string
*/
static char const* gl_error_string(GLenum const err) noexcept
{
    switch (err)
    {
    case GL_NO_ERROR:
        return "GL_NO_ERROR";

    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";

    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";

    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";

    case GL_STACK_OVERFLOW:
        return "GL_STACK_OVERFLOW";

    case GL_STACK_UNDERFLOW:
        return "GL_STACK_UNDERFLOW";

    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";

    case GL_TABLE_TOO_LARGE:
        return "GL_TABLE_TOO_LARGE";

        // opengl 3 errors (1)
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";

        // gles 2, 3 and gl 4 error are handled by the switch above
    default:
        BASE_WARN("unknown error");
        return nullptr;
    }
}

/* All things below are from 'The Cherno' youtube Channel */

static void GLClearError()
{
    static GLenum err;
    uint8_t wd = 255;
    while ((err = glGetError()) != GL_NO_ERROR && wd--) 
    {
    }
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        BASE_ERROR("[OpenGL error]: {0}, {1}): {2} {3}: {4}", error, gl_error_string(error), function, file, line);
        /*std::cerr << "[OpenGL error] (" << error << ", " << gl_error_string(error) << "): " << function <<
            " " << file << ": " << line << std::endl;*/
        return false;
    }
    return true;
}

#if defined(_DEBUG) || defined(DEBUG)
#define GLCall(x) GLClearError();\
     x;\
     BASE_ASSERT(GLLogCall(#x,__FILE__,__LINE__))
#else
#define GLCall(x) x
#endif
#endif