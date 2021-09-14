#pragma once
#include <stdint.h>

namespace Base
{
namespace render
{
	class VertexBuffer
	{
	private:
		uint32_t m_Id = 0;
	public:
		VertexBuffer() = default;
		~VertexBuffer(){}

		void Bind();
		void Unbind();
		void Dispose();

		void SubData(size_t size, const void* buffer, int offset = 0);

		static VertexBuffer CreateVertexBuffer(size_t size);
	};
}
}