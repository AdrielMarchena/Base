#include "CircleRender.h"
#include "utils/gl_error_macro_db.h"
namespace en
{
namespace render
{
	glm::vec2 CircleRender::m_default_tex_coords[] = {
		{0.0f,0.0f},
		{1.0f,0.0f},
		{1.0f,1.0f},
		{0.0f,1.0f}
	};

	static const size_t MaxCircleCount = 1000;
	static const size_t MaxVertexCount = MaxCircleCount * 4;
	static const size_t MaxIndexCount = MaxCircleCount * 6;
	static int32_t MaxTexture = MaxTexturesSlots();

	CircleRender::CircleRender(const char* vs, const char* fs)
	{
		mShader = std::make_shared<Shader>(vs, fs, MaxTexturesSlots());
		mShader->Bind();

		m_data.Buffer = new CircleVertex[MaxVertexCount];

		GLCall(glGenVertexArrays(1, &m_data.VA));
		GLCall(glBindVertexArray(m_data.VA));

		GLCall(glGenBuffers(1, &m_data.VB));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_data.VB));
		GLCall(glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(CircleVertex), nullptr, GL_DYNAMIC_DRAW));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, Position)));

		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, Color)));

		GLCall(glEnableVertexAttribArray(2));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, TexCoords)));

		GLCall(glEnableVertexAttribArray(3));
		GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, TexIndex)));

		GLCall(glEnableVertexAttribArray(4));
		GLCall(glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, MiddlePoint)));

		GLCall(glEnableVertexAttribArray(5));
		GLCall(glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, Rad_Fill_Th)));

		//To much memory for the stack, free below
		uint32_t* indices = new uint32_t[MaxIndexCount]{};
		uint32_t offset = 0;
		for (int i = 0; i < MaxIndexCount; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		GLCall(glGenBuffers(1, &m_data.IB));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data.IB));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _msize(indices), indices, GL_STATIC_DRAW));
		delete[] indices;

		//1x1 white Texture
		GLCall(glGenTextures(1, &m_data.WhiteTexture));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_data.WhiteTexture));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		uint32_t color = 0xffffffff;
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color));

		int32_t* samplers = new int32_t[MaxTexture];
		for (int i = 0; i < MaxTexture; i++)
			samplers[i] = i;
		mShader->SetUniform1iv("u_Textures", MaxTexture, samplers);
		delete[] samplers;

		m_data.TextureSlots = std::vector<uint32_t>(MaxTexture);
		m_data.TextureSlots[0] = m_data.WhiteTexture;
		for (size_t i = 1; i < MaxTexture; i++)
			m_data.TextureSlots[i] = 0;

		m_data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		m_data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		m_data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		m_data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

	}

	void CircleRender::Flush()
	{
		mShader->Bind();
		for (uint8_t i = 0; i < m_data.TextureSlotIndex; i++)
		{
			GLCall(glActiveTexture(GL_TEXTURE0 + i));
			GLCall(glBindTexture(GL_TEXTURE_2D, m_data.TextureSlots[i]));
		}
		Render::Flush();
		m_data.TextureSlotIndex = 1;
	}

	void CircleRender::DrawCircle(const glm::vec3& position, float_t radius, bool fill, float thick, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		float fullSize = radius * 2;
		constexpr float tex_index = 0;

		glm::vec3 quad_position = {position.x - radius, position.y - radius, position.z};
		glm::vec3 quad_size = { position.x + fullSize, position.y + fullSize,position.z };

		glm::vec3 quads[4] = {
			{quad_position.x,quad_position.y,quad_position.z},
			{quad_position.x + quad_size.x, quad_position.y,quad_position.z},
			{quad_position.x + quad_size.x,quad_position.y + quad_size.y,quad_position.z},
			{quad_position.x,quad_position.y + quad_size.y,quad_position.z}
		};

		if (rotation)
			rotate(quads, rotation, { quad_position.x + (quad_size.x / 2), quad_position.y + (quad_size.y / 2), quad_position.z }, axis);

		for (uint8_t i = 0; i < 4; i++)
		{
			m_data.BufferPtr->Position = quads[i];
			m_data.BufferPtr->Color = color;
			m_data.BufferPtr->TexCoords = m_default_tex_coords[i];
			m_data.BufferPtr->TexIndex = tex_index;
			m_data.BufferPtr->MiddlePoint = {position.x,position.y};
			m_data.BufferPtr->Rad_Fill_Th = { radius,fill,thick };
			m_data.BufferPtr++;
		}

		m_data.IndexCount += 6;
	}

	void CircleRender::DrawCircle(const glm::mat4& transform, float_t radius, bool fill, float thick, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		/*for (uint8_t i = 0; i < 4; i++)
		{
			m_data.BufferPtr->Position = trans * m_data.QuadVertexPositions[i];
			m_data.BufferPtr->Color = color;
			m_data.BufferPtr->TexCoords = m_default_tex_coords[i];
			m_data.BufferPtr->TexIndex = tex_index;
			m_data.BufferPtr->MiddlePoint = middle_point;
			m_data.BufferPtr->Rad_Fill_Th = { radius,fill,thick };
			m_data.BufferPtr++;
		}*/

		//FillV(quad_position, quad_size, position, radius, fill, thick, color, m_default_tex_coords, 0, layer, rotation, axis);
		//m_data.IndexCount += 6;
	}
}
}