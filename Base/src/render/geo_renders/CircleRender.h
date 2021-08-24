#pragma once

#include "RenderTemplate.h"

namespace en
{
namespace render
{

	class CircleRender : public Render
	{
	private:
		CircleRenderData m_data;
	public:
		static glm::vec2 m_default_tex_coords[4];

		CircleRender(const char* vs, const char* fs);
		CircleRender(){}
		~CircleRender();

		void BeginBatch();
		void EndBatch();
		void Flush();
		void Dispose();

		const Shader& GetShader();

		void DrawCircle(const glm::vec2& position, float_t radius, bool fill, float thick, const glm::vec4& color,
			float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = {});
	
	private:
		void FillV(const glm::vec2& position, 
				   const glm::vec2& size ,
				   const glm::vec2& middle_point,
				   float_t radius,
				   bool fill,
				   float thick,
				   const glm::vec4& color,
				   const glm::vec2 tex_coords[4], float_t tex_index,
				   float_t layer, float_t rotation, const glm::vec3& axis);
	};
}
}