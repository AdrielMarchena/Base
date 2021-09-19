#pragma once

#include "GL_Types.h"

#include <stdint.h>
namespace Base
{
namespace render
{
	class GLCommands
	{
	public:
		static void GL_DrawElementsCall(GL_Target target, GLsizei size, GL_Type type, const void* indices = nullptr);
		
	};
}
}