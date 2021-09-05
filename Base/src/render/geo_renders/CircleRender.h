#pragma once

#include "RenderTemplate.h"

namespace en
{
namespace render
{

	class CircleRender : public Render<CircleRenderData>
	{
	private:
	public:
		static glm::vec2 m_default_tex_coords[4];

		CircleRender(const char* vs, const char* fs);
		
		void Flush() override;

		void DrawCircle(const glm::vec3& position, float_t radius, bool fill, float thick, const glm::vec4& color,
			float_t rotation = NULL, const glm::vec3& axis = {});
		
		//DO nothing
		void DrawCircle(const glm::mat4& transform,float_t radius ,bool fill, float thick, const glm::vec4& color,
			float_t rotation = NULL, const glm::vec3& axis = {});

	};
}
}