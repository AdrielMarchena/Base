/*
*	@file QuadRender2D.h
*
*	The render it self it's here (header)
*/

#pragma once
#include <array>
#include "glm/glm.hpp"
#include "Shader.h"
#include "Texture.h"
#include "SubTexture.h"
#include <memory>
#include <math.h>

namespace en
{
	namespace render
	{

		struct Stats
		{
			uint32_t DrawCount = 0;
			uint32_t QuadCount = 0;
		};

		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoords;
			float_t TexIndex;
		};

		struct LineVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
		};

		struct RenderData
		{
		public:
			RenderData() {}
			RenderData(const char* vs, const char* fs, int32_t MaxTexSlots)
				:QuadShader(vs, fs, MaxTexSlots),
				LineShader("shaders/line_vs.shader", "shaders/line_fs.shader")
			{
			}

			RenderData& operator=(RenderData& other)
			{
				if (this == &other)
					return *this;

				QuadVA = other.QuadVA;
				QuadVB = other.QuadVB;
				QuadIB = other.QuadIB;
				QuadShader = std::move(other.QuadShader);
				WhiteTexture = other.WhiteTexture;
				WhiteTextureSlot = other.WhiteTextureSlot;
				IndexCount = other.IndexCount;
				QuadBuffer = other.QuadBuffer;
				QuadBufferPtr = other.QuadBufferPtr;

				LineVA = other.LineVA;
				LineVB = other.LineVB;
				LineIB = other.LineIB;
				LineShader = std::move(other.LineShader);
				LineIndexCount = other.LineIndexCount;
				LineBuffer = other.LineBuffer;
				LineBufferPtr = other.LineBufferPtr;
				LineCount = other.LineCount;

				other.QuadVA = NULL;
				other.QuadVB = NULL;
				other.QuadIB = NULL;
				other.WhiteTexture = NULL;
				other.WhiteTextureSlot = NULL;
				other.IndexCount = NULL;
				other.QuadBuffer = nullptr;
				other.QuadBufferPtr = nullptr;

				other.LineVA = NULL;
				other.LineVB = NULL;
				other.LineIB = NULL;
				other.LineIndexCount = NULL;
				other.LineBuffer = nullptr;
				other.LineBufferPtr = nullptr;
				other.LineCount = NULL;

				return *this;
			}
			~RenderData() {}

			uint32_t QuadVA = 0;
			uint32_t QuadVB = 0;
			uint32_t QuadIB = 0;
			Shader   QuadShader;
			uint32_t WhiteTexture = 0;
			uint8_t  WhiteTextureSlot = 0;
			uint32_t IndexCount = 0;
			QuadVertex* QuadBuffer = nullptr;
			QuadVertex* QuadBufferPtr = nullptr;

			std::vector<uint32_t> TextureSlots;
			uint32_t TextureSlotIndex = 1;

			//Line Stuff
			uint32_t LineVA = 0;
			uint32_t LineVB = 0;
			uint32_t LineIB = 0;
			render::Shader   LineShader;
			uint32_t LineIndexCount = 0;
			LineVertex* LineBuffer = nullptr;
			LineVertex* LineBufferPtr = nullptr;
			uint64_t LineCount = 0;

			Stats RenderStatus;
		};

		class QuadRender2D
		{
		private:
			RenderData m_data;

			static glm::vec3 m_default_axis;
			static glm::vec2 m_default_tex_coords[4];
		public:

			QuadRender2D(const char* vs, const char* fs);
			QuadRender2D() {}

			QuadRender2D(QuadRender2D&& other) noexcept
			{
				// =operator of RenderData already do the work
				m_data = other.m_data;
			}

			QuadRender2D& operator=(QuadRender2D&& other) noexcept
			{
				if (this == &other)
					return *this;
				// =operator of RenderData already do the work
				m_data = other.m_data;
				return *this;
			}

			~QuadRender2D();

			void BeginBatch();
			void EndBatch();
			void Flush();

			void LineBeginBatch();
			void LineEndBatch();
			void LineFlush();

			const Shader& GetShader();
			const Shader& GetLineShader();

			void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
				float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

			void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color[4],
				float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

			void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture& texture,
				float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

			void DrawQuad(const glm::vec2& position, const glm::vec2& size, const SubTexture& sub_texture,
				float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

			void DrawOutLineQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
				float_t layer = 0, float_t rotation = NULL, const glm::vec3& axis = m_default_axis);

			[[deprecated("Use DrawLine() instead")]]
			void DrawQuadLine(const glm::vec2& origin, const glm::vec2& dest, const glm::vec4& color, float_t thick,
				float_t layer = 0.0f);

			void DrawLine(const glm::vec2& origin, const glm::vec2& dest, const glm::vec4& color, float_t layer = 0);

		private:
			void rotate(glm::vec3 vertices[4], float rotation, const glm::vec3& rotationCenter, const glm::vec3& axis);

			void FillV(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
				const glm::vec2 tex_coords[4], float_t tex_index,
				float_t layer, float_t rotation, const glm::vec3& axis);

			void FillVC(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color[4],
				const glm::vec2 tex_coords[4], float_t tex_index,
				float_t layer, float_t rotation, const glm::vec3& axis);

		};
	}
}