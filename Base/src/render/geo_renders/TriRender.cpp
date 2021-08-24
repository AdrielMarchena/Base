#include "TriRender.h"
#include "utils/gl_error_macro_db.h"
namespace en
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
	static int32_t MaxTexture = Render::MaxTexturesSlots();

	TriRender::TriRender(const char* vs, const char* fs)
		:m_data(vs, fs, MaxTexture)
	{
		m_data.mShader.Bind();

		m_data.Buffer = new TriangleVertex[MaxVertexCount];

		GLCall(glGenVertexArrays(1, &m_data.VA));
		GLCall(glBindVertexArray(m_data.VA));

		GLCall(glGenBuffers(1, &m_data.VB));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_data.VB));
		GLCall(glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(TriangleVertex), nullptr, GL_DYNAMIC_DRAW));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (const void*)offsetof(TriangleVertex, Position)));

		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (const void*)offsetof(TriangleVertex, Color)));

		GLCall(glEnableVertexAttribArray(2));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (const void*)offsetof(TriangleVertex, TexCoords)));

		GLCall(glEnableVertexAttribArray(3));
		GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (const void*)offsetof(TriangleVertex, TexIndex)));

		//To much memory for the stack, free below
		uint32_t* indices = new uint32_t[MaxIndexCount]{};
		uint32_t offset = 0;
		for (int i = 0; i < MaxIndexCount; i += 3)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			offset += 3;
		}

		GLCall(glGenBuffers(1, &m_data.IB));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data.IB));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _msize(indices), indices, GL_STATIC_DRAW));
		delete[] indices;

		//1x1 white Texture
		GLCall(glGenTextures(1, &m_data.WhiteTexture));
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_data.WhiteTexture));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		uint32_t color = 0xffffffff;
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color));

		int32_t* samplers = new int32_t[MaxTexture];
		for (int i = 0; i < MaxTexture; i++)
			samplers[i] = i;
		m_data.mShader.SetUniform1iv("u_Textures", MaxTexture, samplers);
		delete[] samplers;

		m_data.TextureSlots = std::vector<uint32_t>(MaxTexture);
		m_data.TextureSlots[0] = m_data.WhiteTexture;
		for (size_t i = 1; i < MaxTexture; i++)
			m_data.TextureSlots[i] = 0;
	}

	TriRender::~TriRender()
	{
		GLCall(glDeleteVertexArrays(1, &m_data.VA));
		GLCall(glDeleteBuffers(1, &m_data.VB));
		GLCall(glDeleteBuffers(1, &m_data.IB));
		GLCall(glDeleteTextures(1, &m_data.WhiteTexture));
		delete[] m_data.Buffer;
	}

	void TriRender::BeginBatch()
	{
		m_data.BufferPtr = m_data.Buffer;
	}

	void TriRender::EndBatch()
	{
		m_data.mShader.Bind();
		//Current position - first position
		GLsizeiptr size = (uint8_t*)m_data.BufferPtr - (uint8_t*)m_data.Buffer;
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_data.VB));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_data.Buffer));
	}

	void TriRender::Flush()
	{
		m_data.mShader.Bind();
		for (uint8_t i = 0; i < m_data.TextureSlotIndex; i++)
		{
			GLCall(glActiveTexture(GL_TEXTURE0 + i));
			GLCall(glBindTexture(GL_TEXTURE_2D, m_data.TextureSlots[i]));
		}

		GLCall(glBindVertexArray(m_data.VA));
		GLCall(glDrawElements(GL_TRIANGLES, m_data.IndexCount, GL_UNSIGNED_INT, nullptr));
		m_data.RenderStatus.DrawCount++;

		m_data.IndexCount = 0;
		m_data.TextureSlotIndex = 1;
	}

	const Shader& TriRender::GetShader()
	{
		return m_data.mShader;
	}

	void TriRender::DrawTriangle(const glm::vec2 points[3], const glm::vec4& color, float_t layer)
	{
		FillV(points, color, m_default_tex_coords, 0, layer);
		m_data.IndexCount += 3;
	}

	void TriRender::DrawTriangle(const glm::vec2 points[3], const glm::vec4 color[3], float_t layer)
	{
		FillVC(points, color, m_default_tex_coords, 0, layer);
		m_data.IndexCount += 3;
	}

	void TriRender::FillV(const glm::vec2 position[3], const glm::vec4& color, const glm::vec2 tex_coords[4], float_t tex_index, float_t layer)
	{
		for (uint8_t i = 0; i < 3; i++)
		{
			m_data.BufferPtr->Position = { position[i].x,position[i].y,-layer };
			m_data.BufferPtr->Color = color;
			m_data.BufferPtr->TexCoords = tex_coords[i];
			m_data.BufferPtr->TexIndex = tex_index;
			m_data.BufferPtr++;
		}
	}

	void TriRender::FillVC(const glm::vec2 position[3], const glm::vec4 color[3],
		const glm::vec2 tex_coords[4], float_t tex_index, float_t layer)
	{
		for (uint8_t i = 0; i < 3; i++)
		{
			m_data.BufferPtr->Position = { position[i].x,position[i].y,-layer };
			m_data.BufferPtr->Color = color[i];
			m_data.BufferPtr->TexCoords = tex_coords[i];
			m_data.BufferPtr->TexIndex = tex_index;
			m_data.BufferPtr++;
		}
	}
}
}