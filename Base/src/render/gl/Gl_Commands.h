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
		static void GL_DrawArrayCall(GL_Target mode, int32_t first, int32_t count);
		static void GL_Clear(GL_ClearCommand command = GL_ClearCommand::ClearDepthColor);
	};
}
}