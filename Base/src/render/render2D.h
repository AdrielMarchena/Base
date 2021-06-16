/*
*	@file QuadRender2D.h
*	
*	The render it self it's here (header)
*/

#pragma once
#include <array>
#include "glm/glm.hpp"
#include "Shader.h"
#include "Texture.h"
#include "SubTexture.h"
#include <memory>
#include <math.h>

#include "geo_renders/QuadRender.h"
#include "geo_renders/LineRender.h"

namespace en
{
namespace render
{

	class Render2D
	{
	private:
		QuadRender2D m_QuadRender;
		LineRender2D m_LineRender;
	public:

		static glm::vec3 m_default_axis;

		Render2D(const char* quad_vs, const char* quad_fs,
				 const char* line_vs, const char* line_fs);
		//Render2D(){}

		~Render2D();

		void BeginBatch();
		void EndBatch();
		void Flush();

		const Shader& GetQuadShader();
		const Shader& GetLineShader();

		void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
			float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);
		
		void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color[4],
			float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture& texture,
			float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		void DrawQuad(const glm::vec2& position, const glm::vec2& size, const SubTexture& sub_texture,
			float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		void DrawOutLineQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
			float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		void DrawLine(const glm::vec2& origin, const glm::vec2& dest, const glm::vec4& color, float_t layer = 0);

	private:

		void QuadBeginBatch();
		void QuadEndBatch();
		void QuadFlush();

		void LineBeginBatch();
		void LineEndBatch();
		void LineFlush();

	public: //deprecated
		[[deprecated("Use DrawLine() instead")]]
		void DrawQuadLine(const glm::vec2& origin, const glm::vec2& dest, const glm::vec4& color, float_t thick,
			float_t layer = 0.0f);
	};
}
}