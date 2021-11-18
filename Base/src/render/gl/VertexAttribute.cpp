#include "VertexAttribute.h"

#include "glad/glad.h"
#include "utils/gl_error_macro_db.h"

namespace Base
{
	namespace render
	{

		void VertexAttribute::AddLayoutFloat(int32_t size, uint32_t stride, const void* pointer)
		{
			Vb->Bind();
			GLCall(glEnableVertexAttribArray(current_index));
			GLCall(glVertexAttribPointer(current_index, size, GL_FLOAT, GL_FALSE, stride, pointer));
			current_index++;
			Vb->Unbind();
			
		}

		void VertexAttribute::AddLayoutUint(int32_t size, uint32_t stride, const void* pointer)
		{
			Vb->Bind();
			GLCall(glEnableVertexAttribArray(current_index));
			GLCall(glVertexAttribPointer(current_index, size, GL_UNSIGNED_INT, GL_FALSE, stride, pointer));
			current_index++;
			Vb->Unbind();
		}

		void VertexAttribute::AddLayoutInt(int32_t size, uint32_t stride, const void* pointer)
		{
			Vb->Bind();
			GLCall(glEnableVertexAttribArray(current_index));
			GLCall(glVertexAttribIPointer(current_index, size, GL_INT, stride, pointer));
			current_index++;
			Vb->Unbind();
		}
	}
}