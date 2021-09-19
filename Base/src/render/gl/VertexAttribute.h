#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace Base
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

		void AddLayoutFloat(int32_t size, uint32_t stride,const void* pointer);
		void AddLayoutUint(int32_t size, uint32_t stride,const void* pointer);
	};
}
}