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
#include "Colors.h"
namespace en
{
namespace render
{

	class Render2D
	{
	private:
		/* //On the render2D.cpp file
		static std::shared_ptr<QuadRender2D> m_QuadRender;
		static std::shared_ptr<CircleRender> m_CircleRender;
		static std::shared_ptr<LineRender2D> m_LineRender;
		static std::shared_ptr<QuadRender2D> m_TextRender;
		static std::shared_ptr<TriRender> m_TriRender;
		*/
	public:

		static glm::vec3 m_default_axis;

		static void SetClearColor(const glm::vec4& color);
		static void ClearColor();
		static void ClearColorDepth();

		/*Render2D(const char* quad_vs, const char* quad_fs,
				 const char* line_vs, const char* line_fs,
				 const char* circle_vs, const char* circle_fs,
				 const char* text_vs, const char* text_fs,
				 const char* tri_vs, const char* tri_fs);*/
		//Render2D(){}

		static void Init(const char* quad_vs, const char* quad_fs,
			const char* line_vs, const char* line_fs,
			const char* circle_vs, const char* circle_fs,
			const char* text_vs, const char* text_fs,
			const char* tri_vs, const char* tri_fs);

		static void BeginBatch();
		static void EndBatch();
		static void Flush();
		static void Dispose();

		static const Shader& GetQuadShader();
		static const Shader& GetLineShader();
		static const Shader& GetCircleShader();
		static const Shader& GetTextShader();
		static const Shader& GetTriShader();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
			float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);
		
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color[4],
			float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture& texture,
			float_t layer = 0, const glm::vec4& color = Color::White,float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const SubTexture& sub_texture,
			float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		static void DrawText(const glm::vec2& position, const glm::vec2& size, const Texture& texture,
			float_t layer = 0, const glm::vec4& color = Color::White, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		static void DrawOutLineQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
			float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		static void DrawLine(const glm::vec2& origin, const glm::vec2& dest, const glm::vec4& color, float_t layer = 0);

		static void DrawCurveLine(const glm::vec2& origin, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& dest, const glm::vec4& color, float_t precision = 0.01f ,float_t layer = 0);

		static void DrawCurveLine(const glm::vec2& origin, const glm::vec2& p1, const glm::vec2& dest, const glm::vec4& color, float_t precision = 0.01f, float_t layer = 0);

		static void DrawCircle(const glm::vec2& position, float_t radius, const glm::vec4& color, bool fill = true, float thick = 1.0f,
			float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawTriangle(const glm::vec2 points[3], const glm::vec4& color, float_t layer);

		static void DrawTriangle(const glm::vec2 points[3], const glm::vec4 color[3], float_t layer);

	private:

		static void QuadBeginBatch();
		static void QuadEndBatch();
		static void QuadFlush();

		static void CircleBeginBatch();
		static void CircleEndBatch();
		static void CircleFlush();

		static void LineBeginBatch();
		static void LineEndBatch();
		static void LineFlush();

		static void TextBeginBatch();
		static void TextEndBatch();
		static void TextFlush();

		static void TriBeginBatch();
		static void TriEndBatch();
		static void TriFlush();

	public: //deprecated
		[[deprecated("Use DrawLine() instead")]]
		static void DrawQuadLine(const glm::vec2& origin, const glm::vec2& dest, const glm::vec4& color, float_t thick,
			float_t layer = 0.0f);
	};
}
}