#pragma once
#include "RenderTemplate.h"

#include "render/Texture.h"
namespace en
{
namespace render
{
	class TriRender : public Render<TriRenderData>
	{
	private:
	public:
		static glm::vec2 m_default_tex_coords[4];

		TriRender(const char* vs, const char* fs);
		//TriRender() {}

		void Flush() override;

		void DrawTriangle(const glm::vec3 points[3], const glm::vec4& color);
		void DrawTriangle(const glm::vec3 points[3], const glm::vec4 color[3]);
		//void DrawTriangle(const glm::vec2 points[3], const Texture& texture, float_t layer);
	};
}
}



