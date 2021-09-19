#include "TriRender.h"
#include "utils/gl_error_macro_db.h"
namespace Base
{
namespace render
{
	glm::vec2 TriRender::m_default_tex_coords[] = {
			{0.0f,0.0f},
			{1.0f,0.0f},
			{1.0f,1.0f},
			{0.0f,1.0f}
	};

	static const size_t MaxCircleCount = 1000;
	static const size_t MaxVertexCount = MaxCircleCount * 3;
	static const size_t MaxIndexCount = MaxCircleCount * 3;
	static int32_t MaxTexture = MaxTexturesSlots();

	TriRender::TriRender(const Ref<Shader>& shader)
	{
		mShader = shader;
		mShader->Bind();

		m_data.VerticesNumber = 4;

		m_data.Buffer = new TriangleVertex[MaxVertexCount];

		m_data.VA = VertexArray::CreateVertexArray();

		m_data.VB = VertexBuffer::CreateVertexBuffer(MaxVertexCount * sizeof(TriangleVertex));

		VertexAttribute layout(m_data.VA, m_data.VB);

		layout.AddLayoutFloat(3, sizeof(TriangleVertex), (const void*)offsetof(TriangleVertex, Position));

		layout.AddLayoutFloat(4, sizeof(TriangleVertex), (const void*)offsetof(TriangleVertex, Color));

		layout.AddLayoutFloat(2, sizeof(TriangleVertex), (const void*)offsetof(TriangleVertex, TexCoords));

		layout.AddLayoutFloat(1, sizeof(TriangleVertex), (const void*)offsetof(TriangleVertex, TexIndex));

		uint32_t* indices = new uint32_t[MaxIndexCount]{};
		uint32_t offset = 0;
		for (int i = 0; i < MaxIndexCount; i += 3)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			offset += 3;
		}

		m_data.IB = IndexBuffer::CreateIndexBuffer(_msize(indices), indices);
		delete[] indices;

		SampleTex(MaxTexture);
	}

	void TriRender::Flush()
	{
		if (!m_data.IndexCount)
			return;
		for (uint8_t i = 0; i < m_data.TextureSlotIndex; i++)
		{
			GLCall(glActiveTexture(GL_TEXTURE0 + i));
			GLCall(glBindTexture(GL_TEXTURE_2D, m_data.TextureSlots[i]));
		}

		Render::Flush();
		m_data.TextureSlotIndex = 1;
	}

	void TriRender::DrawTriangle(const glm::vec3 points[3], const glm::vec4& color)
	{
		glm::vec4 colors[3]{};
		for (uint8_t i = 0; i < 3; i++)
			colors[i] = color;
		DrawTriangle(points, colors);
	}

	void TriRender::DrawTriangle(const glm::vec3 points[3], const glm::vec4 color[3])
	{
		for (uint8_t i = 0; i < m_data.VerticesNumber; i++)
		{
			m_data.BufferPtr->Position = { points[i].x,points[i].y,points[i].z };
			m_data.BufferPtr->Color = color[i];
			m_data.BufferPtr->TexCoords = m_default_tex_coords[i];
			m_data.BufferPtr->TexIndex = 0;
			m_data.BufferPtr++;
		}
		m_data.IndexCount += 3;
	}
}
}