#include "IndexBuffer.h"

#include "gl/glew.h"
#include "utils/gl_error_macro_db.h"
namespace Base
{
namespace render
{
	void IndexBuffer::Bind()
	{
		assert(m_Id > 0);
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id));
	}

	void IndexBuffer::Unbind()
	{
		assert(m_Id > 0);
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	void IndexBuffer::Dispose()
	{
		assert(m_Id > 0);
		GLCall(glDeleteBuffers(1, &m_Id));
		m_Id = 0;
	}

	IndexBuffer IndexBuffer::CreateIndexBuffer(size_t size, uint32_t* indices)
	{
		IndexBuffer new_ib;
		GLCall(glGenBuffers(1, &new_ib.m_Id));
		assert(new_ib.m_Id > 0);
		new_ib.Bind();
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW));
		return new_ib;
	}
}
}