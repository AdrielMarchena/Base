#pragma once
#include "RenderTemplate.h"

#include "render/Texture.h"
namespace en
{
namespace render
{
	class TriRender : public Render
	{
	private:
		TriRenderData m_data;
	public:
		static glm::vec2 m_default_tex_coords[4];

		TriRender(const char* vs, const char* fs);
		//QuadRender2D() {}

		TriRender(TriRender&& other) noexcept
		{
			// =operator of RenderData already do the work
			m_data = other.m_data;
		}

		TriRender& operator=(TriRender&& other) noexcept
		{
			if (this == &other)
				return *this;
			// =operator of RenderData already do the work
			m_data = other.m_data;
			return *this;
		}

		~TriRender();

		void BeginBatch();
		void EndBatch();
		void Flush();

		const Shader& GetShader();

		
		void DrawTriangle(const glm::vec2 points[3], const glm::vec4& color, float_t layer);
		void DrawTriangle(const glm::vec2 points[3], const glm::vec4 color[3], float_t layer);
		//void DrawTriangle(const glm::vec2 points[3], const Texture& texture, float_t layer);

	private:
		void FillV(const glm::vec2 position[3], const glm::vec4& color,
			const glm::vec2 tex_coords[4], float_t tex_index,float_t layer);

		void FillVC(const glm::vec2 position[3], const glm::vec4 color[3],
			const glm::vec2 tex_coords[4], float_t tex_index,float_t layer);
	};
}
}



