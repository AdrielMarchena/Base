#pragma once

#include <stdint.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../Shader.h"

#include "gl/glew.h"

namespace en
{
namespace render
{
	struct Stats
	{
		uint32_t DrawCount = 0;
		uint32_t QuadCount = 0;
	};

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct RenderData
	{
	public:
		RenderData() {}

		RenderData(const char* vs, const char* fs, int32_t MaxTexSlots)
			:mShader(vs,fs,MaxTexSlots)
		{
		}

		virtual RenderData& operator=(RenderData& other)
		{
			if (this == &other)
				return *this;

			VA = other.VA;
			VB = other.VB;
			IB = other.IB;
			mShader = std::move(other.mShader);
			IndexCount = other.IndexCount;
			Buffer = other.Buffer;
			BufferPtr = other.BufferPtr;
			Count = other.Count;

			other.VA = NULL;
			other.VB = NULL;
			other.IB = NULL;
			other.IndexCount = NULL;
			other.Buffer = nullptr;
			other.BufferPtr = nullptr;
			other.Count = NULL;

			return *this;
		}
		~RenderData() {}

		uint32_t VA = 0;
		uint32_t VB = 0;
		uint32_t IB = 0;
		Shader mShader;
		uint32_t IndexCount = 0;
		Vertex* Buffer = nullptr;
		Vertex* BufferPtr = nullptr;
		uint32_t Count = 0;

		Stats RenderStatus;
	};

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