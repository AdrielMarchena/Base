#pragma once
#include <stdint.h>
#include <utility>
#include "Base/Base.h"
#include "../gl/Shader.h"
#include "../gl/VertexArray.h"
#include "../gl/VertexBuffer.h"
#include "../gl/VertexAttribute.h"
#include "../gl/IndexBuffer.h"
#include "render/gl/GL_Types.h"
namespace Base
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
		operator Vertex*()
		{
			return this;
		}
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
		float_t Radius;
		float_t Thickness;
		float_t Fade;
	};

	struct TextVertex : public QuadVertex
	{};

	struct RenderData
	{
	public:
		RenderData() {}
		~RenderData() {}

		Ref<VertexArray> VA;
		Ref<VertexBuffer> VB;
		Ref<IndexBuffer> IB;
		uint32_t IndexCount = 0;
		Vertex* Buffer = nullptr;
		Vertex* BufferPtr = nullptr;
		
		uint8_t  WhiteTextureSlot = 0;
		uint32_t Count = 0;
		GL_Target Target = GL_Target::TRIANGLES;
		uint8_t TextureSlotIndex = 1;
		uint8_t VerticesNumber = 2;
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