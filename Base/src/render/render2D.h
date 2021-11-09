#pragma once
#include "Base/Base.h"

#include "glm/glm.hpp"
#include "render/gl/Shader.h"
#include "render/gl/Texture.h"
#include "render/SubTexture.h"
#include "render/Colors.h"
#include "render/Camera.h"
#include <array>
#include <math.h>
namespace Base
{
	class Render2D
	{
	private:
		
	public:
		static constexpr GLbitfield Cl_Color = GL_COLOR_BUFFER_BIT;
		static constexpr GLbitfield Cl_DepthColor = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

		static void SetClearColor(const glm::vec4& color);
		static void ClearColor(GLbitfield clear = Cl_Color);

		static void Init();

		static void AddShader(const std::string& path);

		/*static void SampleTex(Ref<render::Shader> shader, int32_t MaxTexture) //TODO: Move to shader class
		{
			int32_t* samplers = new int32_t[MaxTexture];
			for (int i = 0; i < MaxTexture; i++)
				samplers[i] = i;
			shader->SetUniform1iv("u_Textures", MaxTexture, samplers);
			delete[] samplers;

			m_data.TextureSlots = std::vector<uint32_t>(MaxTexture);
			m_data.TextureSlots[0] = render::Texture::GetWhiteTexture()->GetId();
			for (size_t i = 1; i < MaxTexture; i++)
				m_data.TextureSlots[i] = 0;
		}*/

		static void BeginBatch();
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void EndBatch();
		static void Flush();
		static void Dispose();

		static const Ref<render::Shader> GetQuadShader();
		static const Ref<render::Shader> GetCircleShader();
		static const Ref<render::Shader> GetLineShader();
		//static const Ref<Shader> GetTextShader();
		//static const Ref<Shader> GetTriShader();

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
			float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color[4],
			float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<render::Texture> texture,
			const glm::vec4& color = Color::White, float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const render::SubTexture& sub_texture,
			const glm::vec4& color = Color::White, float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4 color[4]);

		static void DrawQuad(const glm::mat4& transform, Ref<render::Texture> texture, const glm::vec4& color = Color::White);

		static void DrawQuad(const glm::mat4& transform, const render::SubTexture& sub_texture, const glm::vec4& color = Color::White);

		//static void DrawText_(const glm::vec3& position, const glm::vec2& size, Ref<render::Texture> texture,
		//	const glm::vec4& color = Color::White, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		//static void DrawOutLineQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
		//	float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

		static void DrawOutLineQuad(const glm::mat4& transform, const glm::vec4& color);

		static void DrawLine(const glm::vec3& origin, const glm::vec3& dest, const glm::vec4& color);

		static void DrawCurveLine(const glm::vec3& origin, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& dest, const glm::vec4& color, float_t precision = 0.01f);

		static void DrawCurveLine(const glm::vec3& origin, const glm::vec3& p1, const glm::vec3& dest, const glm::vec4& color, float_t precision = 0.01f);

		static void DrawCircle(const glm::vec3& position, float_t radius, float_t fade, float_t thick, const glm::vec4& color, 
			float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawCircle(const glm::vec3& position, float_t radius, float_t fade, float_t thick, Ref<render::Texture> texture,
			const glm::vec4& color, float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawCircle(const glm::vec3& position, float_t radius, float_t fade, float_t thick, const render::SubTexture& sub_texture,
			const glm::vec4& color, float_t rotation = NULL, const glm::vec3& axis = {});

		static void DrawCircle(const glm::mat4& transform, float_t radius, float_t fade, float_t thick, const glm::vec4& color);

		static void DrawCircle(const glm::mat4& transform, float_t radius, float_t fade, float_t thick, Ref<render::Texture> texture,
			const glm::vec4& color);

		static void DrawCircle(const glm::mat4& transform, float_t radius, float_t fade, float_t thick, const render::SubTexture& sub_texture,
			const glm::vec4& color);

		//static void DrawTriangle(const glm::vec3 points[3], const glm::vec4& color);

		//static void DrawTriangle(const glm::vec3 points[3], const glm::vec4 color[3]);

		static void SetLineWidth(float_t thickness = 1.0f);
	private:

	public:
	};
}