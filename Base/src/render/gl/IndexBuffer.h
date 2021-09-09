#pragma once
#include <stdint.h>
namespace en
{
namespace render
{
	class IndexBuffer
	{
	private:
		uint32_t m_Id = 0;
	public:

		IndexBuffer() = default;
		~IndexBuffer(){}

		void Bind();
		void Unbind();
		void Dispose();

		static IndexBuffer CreateIndexBuffer(size_t size, uint32_t* indices);
	};
}
}