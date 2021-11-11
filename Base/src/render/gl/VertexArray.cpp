#pragma once
#include "VertexArray.h"

#include "glad/glad.h"
#include "utils/gl_error_macro_db.h"
namespace Base
{
namespace render
{
	VertexArray::~VertexArray()
	{
		if(m_Id)
			Dispose();
	}
	void VertexArray::Bind()
	{
		BASE_ASSERT(m_Id > 0);
		GLCall(glBindVertexArray(m_Id));
	}

	void VertexArray::Unbind()
	{
		BASE_ASSERT(m_Id > 0);
		GLCall(glBindVertexArray(m_Id));
	}

	void VertexArray::Dispose()
	{
		BASE_ASSERT(m_Id > 0);
		GLCall(glDeleteVertexArrays(1, &m_Id));
		m_Id = 0;
	}

	Ref<VertexArray> VertexArray::CreateVertexArray()
	{
		Ref<VertexArray> new_va = MakeRef<VertexArray>();
		GLCall(glGenVertexArrays(1, &new_va->m_Id));
		BASE_ASSERT(new_va->m_Id > 0);
		new_va->Bind();
		return new_va;
	}
}
}