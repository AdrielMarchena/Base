/*
*	@file QuadRender2D.h
*	
*	The render it self it's here (header)
*/

#pragma once


#include "Base/Base.h"

#include <array>
#include "glm/glm.hpp"
#include "gl/Shader.h"
#include "Texture.h"
#include "SubTexture.h"
#include <math.h>
#include "Colors.h"
#include "Camera.h"

#include "geo_renders/QuadRender.h"
#include "geo_renders/LineRender.h"
#include "geo_renders/CircleRender.h"
#include "geo_renders/TriRender.h"

namespace Base
{
namespace render
{
	class Render2D
	{
	private:
		static Ref<QuadRender2D> m_QuadRender;
		static Ref<CircleRender> m_CircleRender;
		static Ref<LineRender2D> m_LineRender;
		static Ref<QuadRender2D> m_TextRender;
		static Ref<TriRender> m_TriRender;
		static ShaderLib m_Shaders;
	public:
		static constexpr GLbitfield Cl_Color = GL_COLOR_BUFFER_BIT;
		static constexpr GLbitfield Cl_DepthColor = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

		static glm::vec3 m_default_axis;

		static void SetClearColor(const glm::vec4& color);
		static void ClearColor(GLbitfield clear = Cl_Color);

		static void Init();

		static void AddShader(const std::string& path);

		static void BeginBatch();
		static void BeginScene(const Camera& camera,const glm::mat4& transform);
		static void EndBatch();
		static void Flush();
		static void Dispose();

		static void Sort();

		static const Ref<Shader> GetQuadShader();
		static const Ref<Shader> GetLineShader();
		static const Ref<Shader> GetCircleShader();
		static const Ref<Shader> GetTextShader();
		static const Ref<Shader> GetTriShader();

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
			float_t rotation = NULL, const glm::vec3& axis = m_default_axis);
		
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color[4],
			float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture> texture,
			const glm::vec4& color = Color::White,float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const SubTexture& sub_texture,
			const glm::vec4& color = Color::White,float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color = Color::White,
			float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawQuad(const glm::mat4& transform, const glm::vec4 color[4],
			float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawQuad(const glm::mat4& transform, Ref<Texture> texture, const glm::vec4& color = Color::White,
			float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawQuad(const glm::mat4& transform, const SubTexture& sub_texture, const glm::vec4& color = Color::White,
			float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawText_(const glm::vec3& position, const glm::vec2& size, Ref<Texture> texture,
			const glm::vec4& color = Color::White, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		static void DrawOutLineQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
			float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		static void DrawLine(const glm::vec3& origin, const glm::vec3& dest, const glm::vec4& color);

		static void DrawCurveLine(const glm::vec3& origin, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& dest, const glm::vec4& color, float_t precision = 0.01f);

		static void DrawCurveLine(const glm::vec3& origin, const glm::vec3& p1, const glm::vec3& dest, const glm::vec4& color, float_t precision = 0.01f);

		static void DrawCircle(const glm::vec3& position, float_t radius, bool fill, float thick, const glm::vec4& color,
			float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawCircle(const glm::vec3& position, float_t radius, float fill, float thick, Ref<Texture> texture,
			const glm::vec4& color, float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawCircle(const glm::vec3& position, float_t radius, float fill, float thick, const SubTexture& sub_texture,
			const glm::vec4& color, float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawCircle(const glm::mat4& transform, float_t radius, bool fill, float thick, const glm::vec4& color,
			float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawCircle(const glm::mat4& transform, float_t radius, float fill, float thick, Ref<Texture> texture,
			const glm::vec4& color, float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawCircle(const glm::mat4& transform, float_t radius, float fill, float thick, const SubTexture& sub_texture,
			const glm::vec4& color, float_t rotation = NULL, const glm::vec3& axis = {});

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
	};
}
}