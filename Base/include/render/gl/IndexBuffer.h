#pragma once
#include <stdint.h>
#include "Base/Base.h"
#include "GL_Types.h"
namespace Base
{
namespace render
{
	class IndexBuffer
	{
	private:
		uint32_t m_Id = 0;
		GL_Usage m_Usage = GL_Usage::DYNAMIC;
	public:

		IndexBuffer() = default;
		~IndexBuffer();

		void Bind();
		void Unbind();
		void Dispose();

		static Ref<IndexBuffer> CreateIndexBuffer(size_t size, uint32_t* indices,GL_Usage usage = GL_Usage::DYNAMIC);
	};
}
}