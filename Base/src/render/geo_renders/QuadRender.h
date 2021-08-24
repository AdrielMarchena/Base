/*
*	@file QuadRender2D.h
*
*	The render it self it's here (header)
*/

#pragma once

#include "RenderTemplate.h"

#include <array>
#include "glm/glm.hpp"
#include "../Shader.h"
#include "../Texture.h"
#include "../SubTexture.h"
#include <memory>
#include <math.h>

namespace en
{
	namespace render
	{
		class QuadRender2D : public Render
		{
		private:
			QuadRenderData m_data;
		public:
			static glm::vec2 m_default_tex_coords[4];

			QuadRender2D(const char* vs, const char* fs);
			//QuadRender2D() {}

			QuadRender2D(QuadRender2D&& other) noexcept
			{
				// =operator of RenderData already do the work
				m_data = other.m_data;
			}

			QuadRender2D& operator=(QuadRender2D&& other) noexcept
			{
				if (this == &other)
					return *this;
				// =operator of RenderData already do the work
				m_data = other.m_data;
				return *this;
			}

			~QuadRender2D();

			void BeginBatch();
			void EndBatch();
			void Flush();
			void Dispose();

			const Shader& GetShader();

			void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
				float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = {});

			void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color[4],
				float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = {});

			void DrawQuad(const glm::vec2& position, const glm::vec2& size,
				const Texture& texture, const glm::vec4& color = { 1.0f,1.0f,1.0f,1.0f }, float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = {});

			void DrawQuad(const glm::vec2& position, const glm::vec2& size, const SubTexture& sub_texture,
				float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = {});

			void DrawQuadLine(const glm::vec2& origin, const glm::vec2& dest, const glm::vec4& color, float_t thick,
				float_t layer = 0.0f);

		private:
			void FillV(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
				const glm::vec2 tex_coords[4], float_t tex_index,
				float_t layer, float_t rotation, const glm::vec3& axis);

			void FillVC(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color[4],
				const glm::vec2 tex_coords[4], float_t tex_index,
				float_t layer, float_t rotation, const glm::vec3& axis);
		};
	}
}