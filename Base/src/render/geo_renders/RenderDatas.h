#pragma once
#include <stdint.h>
#include <utility>
#include "../Shader.h"

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

	struct LineVertex : Vertex
	{
	};

	struct QuadVertex : public Vertex
	{
		glm::vec2 TexCoords;
		float_t TexIndex;
	};

	struct CircleVertex : public QuadVertex
	{
		glm::vec2 MiddlePoint;
		glm::vec3 Rad_Fill_Th;
		//float Radius;
		//float Fill;
	};


	struct TextVertex : public QuadVertex
	{};

	struct RenderData
	{
	public:
		RenderData() {}

		RenderData(const char* vs, const char* fs, int32_t MaxTexSlots)
			:mShader(vs, fs, MaxTexSlots)
		{
		}

		virtual RenderData& operator=(RenderData& other)
		{
			if (this == &other)
				return *this;

			if(Buffer)
				delete[] Buffer;

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

	struct QuadRenderData : RenderData
	{
	public:
		QuadRenderData() {}
		QuadRenderData(const char* vs, const char* fs, int32_t MaxTexSlots)
			:RenderData(vs, fs, MaxTexSlots)
		{
		}

		QuadRenderData& operator=(QuadRenderData& other)
		{
			if (this == &other)
				return *this;

			RenderData::operator= (other);

			WhiteTexture = other.WhiteTexture;
			WhiteTextureSlot = other.WhiteTextureSlot;
			TextureSlots = other.TextureSlots;
			TextureSlotIndex = other.TextureSlotIndex;

			other.WhiteTexture = NULL;
			other.WhiteTextureSlot = NULL;
			other.TextureSlots.clear();
			other.TextureSlotIndex = 1;

			return *this;
		}
		~QuadRenderData() {}

		uint32_t WhiteTexture = 0;
		uint8_t  WhiteTextureSlot = 0;

		std::vector<uint32_t> TextureSlots;
		uint8_t TextureSlotIndex = 1;

		//Hide the Vertex* with the QuadVertex*
		QuadVertex* Buffer = nullptr;
		QuadVertex* BufferPtr = nullptr;

		Stats RenderStatus;
	};

	struct CircleRenderData : public QuadRenderData
	{
		CircleRenderData() {}
		CircleRenderData(const char* vs, const char* fs, int32_t MaxTexSlots)
			:QuadRenderData(vs, fs, MaxTexSlots)
		{
		}

		CircleRenderData& operator=(CircleRenderData& other)
		{
			if (this == &other)
				return *this;

			QuadRenderData::operator= (other);

			return *this;
		}
		~CircleRenderData() {}

		CircleVertex* Buffer = nullptr;
		CircleVertex* BufferPtr = nullptr;

	};

}
}