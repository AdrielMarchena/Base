#pragma once

#include <stdint.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "RenderDatas.h"

#include "gl/glew.h"

namespace en
{
namespace render
{

	class Render
	{
	public:
		virtual void BeginBatch() = 0;
		virtual void EndBatch() = 0;
		virtual void Flush() = 0;

		virtual const Shader& GetShader() = 0;

		static int32_t MaxTexturesSlots()
		{
			static int32_t MaxT = 8;
			glGetIntegerv(GL_MAX_TEXTURE_UNITS, &MaxT);
			return MaxT;
		}

		static void rotate(glm::vec3 vertices[4], float rotation, const glm::vec3& rotationCenter, const glm::vec3& axis)
		{
			const glm::mat4 translationMatrix = glm::translate(glm::identity<glm::mat4>(), -rotationCenter);
			const glm::mat4 rotationMatrix = glm::rotate(glm::identity<glm::mat4>(), rotation, axis);
			const glm::mat4 reverseTranslationMatrix = glm::translate(glm::identity<glm::mat4>(), rotationCenter);

			for (size_t i = 0; i < 4; i++) {
				vertices[i] = glm::vec3(
					reverseTranslationMatrix * rotationMatrix * translationMatrix * glm::vec4(vertices[i], 1.0f));
			}
		}
	};

}
}