#pragma once
#include <stdint.h>

//TODO: Remove typedefs
typedef unsigned int GLenum;
typedef int GLsizei;
typedef unsigned int GLuint;
typedef ptrdiff_t GLsizeiptr;
namespace Base
{
namespace render
{
	//Data types
	enum GL_Type: uint8_t
	{
		UNSIGNED_INT = 0
	};

	//Enum types
	enum GL_Target: uint8_t
	{
		TRIANGLES = 0,
		LINES,
		LINE
	};
	unsigned int GetType(GL_Type type);
	unsigned int GetTarget(GL_Target type);
}
}