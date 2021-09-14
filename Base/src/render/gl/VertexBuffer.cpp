#include "VertexBuffer.h"

#include "gl/glew.h"
#include "utils/gl_error_macro_db.h"
namespace Base
{
namespace render
{
	void VertexBuffer::Bind()
	{
		assert(m_Id > 0);
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Id));
	}

	void VertexBuffer::Unbind()
	{
		assert(m_Id > 0);
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void VertexBuffer::Dispose()
	{
		assert(m_Id > 0);
		GLCall(glDeleteBuffers(1, &m_Id));
		m_Id = 0;
	}

	void VertexBuffer::SubData(size_t size, const void* buffer, int offset)
	{
		assert(m_Id > 0);
		Bind();
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, buffer));
	}

	VertexBuffer VertexBuffer::CreateVertexBuffer(size_t size)
	{
		VertexBuffer new_vb;
		GLCall(glGenBuffers(1, &new_vb.m_Id));
		assert(new_vb.m_Id > 0);
		new_vb.Bind();
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
		return new_vb;
	}
}
}