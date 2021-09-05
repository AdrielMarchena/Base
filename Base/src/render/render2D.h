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
#include "Camera.h"

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
		static std::shared_ptr<QuadRender2D> m_QuadRender;
		static std::shared_ptr<CircleRender> m_CircleRender;
		static std::shared_ptr<LineRender2D> m_LineRender;
		static std::shared_ptr<QuadRender2D> m_TextRender;
		static std::shared_ptr<TriRender> m_TriRender;
		
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
		static void BeginScene(const Camera& camera,const glm::mat4& transform);
		static void EndBatch();
		static void Flush();
		static void Dispose();

		//Temporary
		static void SetTransform(glm::mat4 transform)
		{
			m_Transform = transform;
		}
		static const glm::mat4& GetTransform() { return m_Transform; }

		static const std::shared_ptr<Shader> GetQuadShader();
		static const std::shared_ptr<Shader> GetLineShader();
		static const std::shared_ptr<Shader> GetCircleShader();
		static const std::shared_ptr<Shader> GetTextShader();
		static const std::shared_ptr<Shader> GetTriShader();

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
			float_t rotation = NULL, const glm::vec3& axis = m_default_axis);
		
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color[4],
			float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Texture& texture,
			const glm::vec4& color = Color::White,float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const SubTexture& sub_texture,
			const glm::vec4& color = Color::White,float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color = Color::White,
			float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawQuad(const glm::mat4& transform, const glm::vec4 color[4],
			float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawQuad(const glm::mat4& transform, const Texture& texture, const glm::vec4& color = Color::White,
			float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawQuad(const glm::mat4& transform, const SubTexture& sub_texture, const glm::vec4& color = Color::White,
			float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawText(const glm::vec3& position, const glm::vec2& size, const Texture& texture,
			const glm::vec4& color = Color::White, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		static void DrawOutLineQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
			float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		static void DrawLine(const glm::vec3& origin, const glm::vec3& dest, const glm::vec4& color);

		static void DrawCurveLine(const glm::vec3& origin, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& dest, const glm::vec4& color, float_t precision = 0.01f);

		static void DrawCurveLine(const glm::vec3& origin, const glm::vec3& p1, const glm::vec3& dest, const glm::vec4& color, float_t precision = 0.01f);

		static void DrawCircle(const glm::vec3& position, float_t radius, const glm::vec4& color, bool fill = true, float thick = 1.0f,
			float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawTriangle(const glm::vec3 points[3], const glm::vec4& color);

		static void DrawTriangle(const glm::vec3 points[3], const glm::vec4 color[3]);

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

		//Temporary
		static glm::mat4 m_Transform;
	};
}
}