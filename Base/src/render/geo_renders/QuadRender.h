/*
*	@file QuadRender2D.h
*
*	The render it self it's here (header)
*/

#pragma once

#include "RenderTemplate.h"

#include <array>
#include "glm/glm.hpp"
#include "render/gl/Texture.h"
#include "render/SubTexture.h"
#include <memory>
#include <math.h>

namespace Base
{
	namespace render
	{
		class QuadRender2D : public Render<QuadRenderData>
		{
		private:
		public:
			static glm::vec2 m_default_tex_coords[4];

			QuadRender2D(const Ref<Shader>& shader);
			QuadRender2D() = default;

			void Flush() override;

			void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
				 float_t rotation = NULL, const glm::vec3& axis = {});

			void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color[4],
				 float_t rotation = NULL, const glm::vec3& axis = {});

			void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture> texture,
				 const glm::vec4& color, float_t rotation = NULL, const glm::vec3& axis = {});

			void DrawQuad(const glm::vec3& position, const glm::vec2& size, const SubTexture& sub_texture,
				 const glm::vec4& color,float_t rotation = NULL, const glm::vec3& axis = {});

			void DrawQuad(const glm::mat4& transform, const glm::vec4& color, 
				 float_t rotation = NULL, const glm::vec3& axis = {});

			void DrawQuad(const glm::mat4& transform, const glm::vec4 color[4],
				 float_t rotation = NULL, const glm::vec3& axis = {});

			void DrawQuad(const glm::mat4& transform, Ref<Texture> texture ,const glm::vec4& color,
				float_t rotation = NULL, const glm::vec3& axis = {});

			void DrawQuad(const glm::mat4& transform, const SubTexture& sub_texture, const glm::vec4& color,
				float_t rotation = NULL, const glm::vec3& axis = {});

			void DrawQuadLine(const glm::vec2& origin, const glm::vec2& dest, const glm::vec4& color, 
				float_t thick, float_t layer = 0.0f);

		private:
			friend Render2D;
		};
	}
}