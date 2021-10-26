#pragma once

#include "RenderTemplate.h"

#include "render/gl/Texture.h"
#include "render/SubTexture.h"

namespace Base
{
namespace render
{
	class CircleRender : public Render<CircleRenderData>
	{
	private:
	public:
		static glm::vec2 m_default_tex_coords[4];

		CircleRender(const Ref<Shader>& shader);
		CircleRender() = default;

		void Flush() override;

		void DrawCircle(const glm::vec3& position, float_t radius, float_t fade, float_t thick, const glm::vec4& color,
			float_t rotation = NULL, const glm::vec3& axis = {});
		
		void DrawCircle(const glm::vec3& position, float_t radius, float_t fade, float_t thick, Ref<Texture> texture,
			const glm::vec4& color, float_t rotation = NULL, const glm::vec3& axis = {});

		void DrawCircle(const glm::vec3& position, float_t radius, float_t fade, float_t thick, const SubTexture& sub_texture,
			const glm::vec4& color, float_t rotation = NULL, const glm::vec3& axis = {});

		void DrawCircle(const glm::mat4& transform, float_t radius, float_t fade, float_t thick, const glm::vec4& color,
			float_t rotation = NULL, const glm::vec3& axis = {});

		void DrawCircle(const glm::mat4& transform, float_t radius, float_t fade, float_t thick, Ref<Texture> texture,
			const glm::vec4& color, float_t rotation = NULL, const glm::vec3& axis = {});

		void DrawCircle(const glm::mat4& transform, float_t radius, float_t fade, float_t thick, const SubTexture& sub_texture,
			const glm::vec4& color, float_t rotation = NULL, const glm::vec3& axis = {});

		/*//DO nothing
		void DrawCircle(const glm::mat4& transform,float_t radius ,bool fill, float thick, const glm::vec4& color,
			float_t rotation = NULL, const glm::vec3& axis = {});*/

	};
}
}