/*
*	@file LineRender2D.h
*
*	The render it self it's here (header)
*/

#pragma once
#include <array>
#include "glm/glm.hpp"
#include <memory>
#include <math.h>
#include "RenderTemplate.h"

namespace Base
{
	namespace render
	{
		class LineRender2D : public Render<RenderData>
		{
		private:
		public:

			LineRender2D(const Ref<Shader>& shader);
			LineRender2D() = default;

			void DrawLine(const glm::vec3& origin, const glm::vec3& dest, const glm::vec4& color);
			void DrawCurveLine(const glm::vec3& origin, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& dest, const glm::vec4& color, float_t precision);
			void DrawCurveLine(const glm::vec3& origin, const glm::vec3& p1, const glm::vec3& dest, const glm::vec4& color, float_t precision);

		};
	}
}