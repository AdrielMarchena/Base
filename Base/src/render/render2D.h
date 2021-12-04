#pragma once
#include "Base/Base.h"

#include "render/gl/Shader.h"
#include "render/gl/Texture.h"
#include "render/SubTexture.h"
#include "render/Colors.h"
#include "render/Camera.h"
#include "render/Text.h"

#include "gl/Gl_Commands.h"

#include "scene/EditorCamera.h"

#include <array>
#include "glm/glm.hpp"
#include <math.h>
namespace Base
{
	struct RenderStats
	{
		uint64_t TotalCount = 0;
		uint32_t DrawCount = 0;
	};

	class Render2D
	{
	private:
		
	public:

		static void SetClearColor(const glm::vec4& color);
		static void ClearColor(GL_ClearCommand clear = GL_ClearCommand::ClearDepthColor);

		static void Init();

		static void AddShader(const std::string& path);

		static void BeginBatch();
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndBatch();
		static void EndScene();
		static void Flush();
		static void Dispose();

		static const Ref<render::Shader> GetQuadShader();
		static const Ref<render::Shader> GetCircleShader();
		static const Ref<render::Shader> GetLineShader();
		//static const Ref<Shader> GetTextShader();
		//static const Ref<Shader> GetTriShader();

		static RenderStats GetQuadStats();
		static RenderStats GetCircleStats();
		static RenderStats GetLineStats();
		static uint64_t GetDrawCallsCount();

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int32_t entityID,
			float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color[4], int32_t entityID,
			float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<render::Texture> texture, int32_t entityID,
			const glm::vec4& color = Color::White, float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const render::SubTexture& sub_texture, int32_t entityID,
			const glm::vec4& color = Color::White, float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int32_t entityID);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4 color[4], int32_t entityID);

		static void DrawQuad(const glm::mat4& transform, Ref<render::Texture> texture,int32_t entityID, const glm::vec4& color = Color::White);

		static void DrawQuad(const glm::mat4& transform, const render::SubTexture& sub_texture, int32_t entityID, const glm::vec4& color = Color::White);

		//static void DrawText_(const glm::vec3& position, const glm::vec2& size, Ref<render::Texture> texture,
		//	const glm::vec4& color = Color::White, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		//static void DrawOutLineQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
		//	float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		static void DrawOutLineQuad(const glm::mat4& transform, const glm::vec4& color, int32_t entityID);

		static void DrawLine(const glm::vec3& origin, const glm::vec3& dest, const glm::vec4& color, int32_t entityID);

		static void DrawCurveLine(const glm::vec3& origin, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& dest, const glm::vec4& color, int32_t entityID, float_t precision = 0.01f);

		static void DrawCurveLine(const glm::vec3& origin, const glm::vec3& p1, const glm::vec3& dest, const glm::vec4& color, int32_t entityID, float_t precision = 0.01f);

		static void DrawCircle(const glm::vec3& position, float_t radius, float_t fade, float_t thick, const glm::vec4& color, int32_t entityID,
			float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawCircle(const glm::vec3& position, float_t radius, float_t fade, float_t thick, Ref<render::Texture> texture, int32_t entityID,
			const glm::vec4& color = Color::White, float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawCircle(const glm::vec3& position, float_t radius, float_t fade, float_t thick, const render::SubTexture& sub_texture, int32_t entityID,
			const glm::vec4& color = Color::White, float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawCircle(const glm::mat4& transform, float_t radius, float_t fade, float_t thick, const glm::vec4& color, int32_t entityID);

		static void DrawCircle(const glm::mat4& transform, float_t radius, float_t fade, float_t thick, Ref<render::Texture> texture, int32_t entityID,
			const glm::vec4& color = Color::White);

		static void DrawCircle(const glm::mat4& transform, float_t radius, float_t fade, float_t thick, const render::SubTexture& sub_texture, int32_t entityID,
			const glm::vec4& color = Color::White);

		static void DrawFont(const glm::mat4& transform, const std::string& text, Ref<Font> font, const glm::vec4& color, int32_t entityID);

		//static void DrawTriangle(const glm::vec3 points[3], const glm::vec4& color);

		//static void DrawTriangle(const glm::vec3 points[3], const glm::vec4 color[3]);

		static void SetLineWidth(float_t thickness = 1.0f);
	private:

	public:
	};
}