#include "Gl_Commands.h"

#include "gl/glew.h"
#include "utils/gl_error_macro_db.h"

namespace Base
{
namespace render
{
	void GLCommands::GL_DrawElementsCall(GL_Target target, GLsizei size, GL_Type type, const void* indices)
	{
		GLCall(glDrawElements(GetTarget(target), size, GetType(type), indices));
	}
}
}