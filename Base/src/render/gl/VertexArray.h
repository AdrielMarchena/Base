#pragma once
#include <stdint.h>
namespace Base
{
	namespace render
	{
		class VertexArray
		{
		private:
			uint32_t m_Id = 0;
		public:
			VertexArray() = default;
			~VertexArray() {}

			void Bind();
			void Unbind();
			void Dispose();

			static VertexArray CreateVertexArray();
		};
	}
}