/*
*	@file LineRender2D.h
*
*	The render it self it's here (header)
*/

#pragma once
#include <array>
#include "glm/glm.hpp"
#include "../Shader.h"
#include "../Texture.h"
#include "../SubTexture.h"
#include <memory>
#include <math.h>
#include "RenderTemplate.h"

namespace en
{
	namespace render
	{
		class LineRender2D : public Render
		{
		private:
			RenderData m_data;
		public:

			LineRender2D(const char* vs, const char* fs);
			//LineRender2D() {}

			LineRender2D(LineRender2D&& other) noexcept
			{
				// =operator of RenderData already do the work
				m_data = other.m_data;
			}

			LineRender2D& operator=(LineRender2D&& other) noexcept
			{
				if (this == &other)
					return *this;
				// =operator of RenderData already do the work
				m_data = other.m_data;
				return *this;
			}

			~LineRender2D();

			void BeginBatch();
			void EndBatch();
			void Flush();

			const Shader& GetShader();

			void DrawLine(const glm::vec2& origin, const glm::vec2& dest, const glm::vec4& color, float_t layer = 0);
		};
	}
}