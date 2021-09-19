#include "GL_Types.h"
#include "Log.h"
#include "gl/glew.h"
#include "utils/base_assert.h"
namespace Base
{
namespace render
{
	unsigned int GetType(GL_Type type)
	{
		switch (type)
		{
		case GL_Type::UNSIGNED_INT:
			return GL_UNSIGNED_INT;
			break;
		default:
			BASE_CORE_ASSERT(false, "Type not reconized");
			break;
		}
	}
	unsigned int GetTarget(GL_Target type)
	{
		switch (type)
		{
		case GL_Target::TRIANGLES:
			return GL_TRIANGLES;
			break;
		case GL_Target::LINE:
			return GL_LINE;
			break;
		case GL_Target::LINES:
			return GL_LINES;
			break;
		default:
			BASE_CORE_ASSERT(false, "Target not reconized");
			break;
		}
	}
}
}