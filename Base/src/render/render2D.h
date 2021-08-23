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
#include "geo_renders/CircleRender.h"
#include "geo_renders/TriRender.h"

namespace en
{
namespace render
{

	class Render2D
	{
	private:
		QuadRender2D m_QuadRender;
		CircleRender m_CircleRender;
		LineRender2D m_LineRender;
		QuadRender2D m_TextRender;
		TriRender m_TriRender;
	public:

		static glm::vec3 m_default_axis;

		static inline void SetClearColor(const glm::vec4& color);
		static inline void ClearColor();
		static inline void ClearColorDepth();

		Render2D(const char* quad_vs, const char* quad_fs,
				 const char* line_vs, const char* line_fs,
				 const char* circle_vs, const char* circle_fs,
				 const char* text_vs, const char* text_fs,
				 const char* tri_vs, const char* tri_fs);
		//Render2D(){}

		~Render2D();

		void BeginBatch();
		void EndBatch();
		void Flush();

		const Shader& GetQuadShader();
		const Shader& GetLineShader();
		const Shader& GetCircleShader();
		const Shader& GetTextShader();
		const Shader& GetTriShader();

		void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
			float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);
		
		void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color[4],
			float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture& texture,
			float_t layer = 0, const glm::vec4& color = Color::White,float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		void DrawQuad(const glm::vec2& position, const glm::vec2& size, const SubTexture& sub_texture,
			float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		void DrawText(const glm::vec2& position, const glm::vec2& size, const Texture& texture,
			float_t layer = 0, const glm::vec4& color = Color::White, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		void DrawOutLineQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
			float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		void DrawLine(const glm::vec2& origin, const glm::vec2& dest, const glm::vec4& color, float_t layer = 0);

		void DrawCurveLine(const glm::vec2& origin, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& dest, const glm::vec4& color, float_t precision = 0.01f ,float_t layer = 0);

		void DrawCurveLine(const glm::vec2& origin, const glm::vec2& p1, const glm::vec2& dest, const glm::vec4& color, float_t precision = 0.01f, float_t layer = 0);

		void DrawCircle(const glm::vec2& position, float_t radius, const glm::vec4& color, bool fill = true, float thick = 1.0f,
			float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = {});

		void DrawTriangle(const glm::vec2 points[3], const glm::vec4& color, float_t layer);

		void DrawTriangle(const glm::vec2 points[3], const glm::vec4 color[3], float_t layer);

	private:

		void QuadBeginBatch();
		void QuadEndBatch();
		void QuadFlush();

		void CircleBeginBatch();
		void CircleEndBatch();
		void CircleFlush();

		void LineBeginBatch();
		void LineEndBatch();
		void LineFlush();

		void TextBeginBatch();
		void TextEndBatch();
		void TextFlush();

		void TriBeginBatch();
		void TriEndBatch();
		void TriFlush();

	public: //deprecated
		[[deprecated("Use DrawLine() instead")]]
		void DrawQuadLine(const glm::vec2& origin, const glm::vec2& dest, const glm::vec4& color, float_t thick,
			float_t layer = 0.0f);
	};
}
}