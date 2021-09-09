#pragma once
#include "VertexArray.h"

#include "gl/glew.h"
#include "utils/gl_error_macro_db.h"
namespace en
{
namespace render
{
	void VertexArray::Bind()
	{
		assert(m_Id > 0);
		GLCall(glBindVertexArray(m_Id));
	}

	void VertexArray::Unbind()
	{
		assert(m_Id > 0);
		GLCall(glBindVertexArray(m_Id));
	}

	void VertexArray::Dispose()
	{
		assert(m_Id > 0);
		GLCall(glDeleteVertexArrays(1, &m_Id));
		m_Id = 0;
	}

	VertexArray VertexArray::CreateVertexArray()
	{
		VertexArray new_va;
		GLCall(glGenVertexArrays(1, &new_va.m_Id));
		assert(new_va.m_Id > 0);
		new_va.Bind();
		return new_va;
	}
}
}