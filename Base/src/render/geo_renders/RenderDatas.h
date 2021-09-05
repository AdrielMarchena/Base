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

	struct TriangleVertex : public Vertex
	{
		glm::vec2 TexCoords;
		float_t TexIndex;
	};

	struct QuadVertex : public TriangleVertex
	{
	};

	struct CircleVertex : public QuadVertex
	{
		glm::vec2 MiddlePoint;
		glm::vec3 Rad_Fill_Th;
	};

	struct TextVertex : public QuadVertex
	{};

	struct RenderData
	{
	public:
		RenderData() {}
		~RenderData() {}

		uint32_t VA = 0;
		uint32_t VB = 0;
		uint32_t IB = 0;
		uint32_t IndexCount = 0;
		Vertex* Buffer = nullptr;
		Vertex* BufferPtr = nullptr;
		uint32_t WhiteTexture = 0;
		uint8_t  WhiteTextureSlot = 0;
		uint32_t Count = 0;
		GLenum Target = GL_TRIANGLES;
		uint8_t TextureSlotIndex = 1;
		Stats RenderStatus;
	};

	struct TriRenderData : public RenderData
	{
	public:
		TriRenderData(){}
		~TriRenderData() {}

		std::vector<uint32_t> TextureSlots;

		//Hide the Vertex* with the TriVertex*
		TriangleVertex* Buffer = nullptr;
		TriangleVertex* BufferPtr = nullptr;
	};

	struct QuadRenderData : public TriRenderData
	{
		QuadRenderData() {}
		~QuadRenderData() {}

		//Hide the TriVertex* with the QuadVertex*
		QuadVertex* Buffer = nullptr;
		QuadVertex* BufferPtr = nullptr;
		glm::vec4 QuadVertexPositions[4]{};
	};



	struct CircleRenderData : public QuadRenderData
	{
		//CircleRenderData() {}
		CircleRenderData() {}
		~CircleRenderData() {}

		CircleVertex* Buffer = nullptr;
		CircleVertex* BufferPtr = nullptr;
	};

}
}