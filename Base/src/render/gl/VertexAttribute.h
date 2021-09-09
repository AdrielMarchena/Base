#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"

#include "gl/glew.h"
#include "utils/gl_error_macro_db.h"
namespace en
{
namespace render
{
	class VertexAttribute
	{
		VertexArray* Va = nullptr;
		VertexBuffer* Vb = nullptr;
		uint32_t current_index = 0;
	public:
		VertexAttribute(VertexArray va, VertexBuffer vb)
			:Va(&va),Vb(&vb) {}

		template<typename T>
		void AddLayout(GLint size, GLsizei stride, const void* pointer)
		{
			//Va->Bind();
			//Vb->Bind();
			GLCall(glEnableVertexAttribArray(current_index));
			GLCall(glVertexAttribPointer(current_index, size, GL_FLOAT, GL_FALSE, stride, pointer));
			current_index++;
		}
		template<>
		void AddLayout<float>(GLint size, GLsizei stride, const void* pointer)
		{
			//Va->Bind();
			//Vb->Bind();
			GLCall(glEnableVertexAttribArray(current_index));
			GLCall(glVertexAttribPointer(current_index, size, GL_FLOAT, GL_FALSE, stride, pointer));
			current_index++;
		}
		template<>
		void AddLayout<unsigned int>(GLint size, GLsizei stride, const void* pointer)
		{
			//Va->Bind();
			//Vb->Bind();
			GLCall(glEnableVertexAttribArray(current_index));
			GLCall(glVertexAttribPointer(current_index, size, GL_UNSIGNED_INT, GL_FALSE, stride, pointer));
			current_index++;
		}
	};
}
}