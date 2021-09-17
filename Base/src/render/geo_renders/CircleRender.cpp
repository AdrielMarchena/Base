#include "CircleRender.h"
#include "utils/gl_error_macro_db.h"

namespace Base
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

	CircleRender::CircleRender(const Ref<Shader>& shader)
	{
		mShader = shader;
		mShader->Bind();

		m_data.VerticesNumber = 4;

		m_data.Buffer = new CircleVertex[MaxVertexCount];

		m_data.VA = VertexArray::CreateVertexArray();

		m_data.VB = VertexBuffer::CreateVertexBuffer(MaxVertexCount * sizeof(CircleVertex));

		VertexAttribute layout(m_data.VA, m_data.VB);
		layout.AddLayout<float>(3, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, Position));

		layout.AddLayout<float>(4, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, Color));

		layout.AddLayout<float>(2, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, TexCoords));

		layout.AddLayout<float>(1, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, TexIndex));

		layout.AddLayout<float>(2, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, MiddlePoint));

		layout.AddLayout<float>(3, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, Rad_Fill_Th));

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

		m_data.IB = IndexBuffer::CreateIndexBuffer(_msize(indices), indices);
		delete[] indices;

		SampleTex(MaxTexture);

		m_data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		m_data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		m_data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		m_data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void CircleRender::Flush()
	{
		if (!m_data.IndexCount)
			return;
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
		glm::vec3 quad_position = { position.x, position.y, position.z };
		glm::vec3 quad_size = { radius, radius,position.z };
		DrawCircle(pos_trans(quad_position, quad_size),radius,fill,thick,color,rotation,axis);
	}

	void CircleRender::DrawCircle(const glm::vec3& position, float_t radius, float fill, float thick, Ref<Texture> texture, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		glm::vec3 quad_position = { position.x, position.y, position.z };
		glm::vec3 quad_size = { radius, radius,position.z };
		DrawCircle(pos_trans(quad_position, quad_size), radius, fill, thick, texture ,color, rotation, axis);
	}

	void CircleRender::DrawCircle(const glm::vec3& position, float_t radius, float fill, float thick, const SubTexture& sub_texture, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		glm::vec3 quad_position = { position.x, position.y, position.z };
		glm::vec3 quad_size = { radius, radius,position.z };
		DrawCircle(pos_trans(quad_position, quad_size), radius, fill, thick, sub_texture, color, rotation, axis);
	}

	void CircleRender::DrawCircle(const glm::mat4& transform, float_t radius, bool fill, float thick, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		if (m_data.IndexCount >= MaxIndexCount)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}
		float diameter = radius * 2;
		constexpr float tex_index = 0;

		glm::mat4 trans = transform;
		if (rotation)
			trans = glm::rotate(trans, glm::radians(rotation), axis);

		for (uint8_t i = 0; i < m_data.VerticesNumber; i++)
		{
			m_data.BufferPtr->Position = trans * m_data.QuadVertexPositions[i];
			m_data.BufferPtr->Color = color;
			m_data.BufferPtr->TexCoords = m_default_tex_coords[i];
			m_data.BufferPtr->TexIndex = tex_index; 
			m_data.BufferPtr->MiddlePoint = glm::vec2(BASE_XPOS(trans), BASE_YPOS(trans));
			m_data.BufferPtr->Rad_Fill_Th = { radius,fill,thick };
			m_data.BufferPtr++;
		}

		m_data.IndexCount += 6;
	}
	
	void CircleRender::DrawCircle(const glm::mat4& transform, float_t radius, float fill, float thick, Ref<Texture> texture, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		if (m_data.IndexCount >= MaxIndexCount || m_data.TextureSlotIndex > MaxTexture - 1)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		int8_t texture_index = 0;
		if (texture->GetId())
			for (int8_t i = 1; i < m_data.TextureSlotIndex; i++)
			{
				if (m_data.TextureSlots[i] == texture->GetId())
				{
					texture_index = i;
					break;
				}
			}

		if (texture->GetId())
			if (!texture_index)
			{
				texture_index = m_data.TextureSlotIndex;
				m_data.TextureSlots[m_data.TextureSlotIndex] = texture->GetId();
				m_data.TextureSlotIndex++;
			}

		glm::mat4 trans = transform;
		if (rotation)
			trans = glm::rotate(transform, glm::radians(rotation), axis);

		for (size_t i = 0; i < m_data.VerticesNumber; i++)
		{
			m_data.BufferPtr->Position = trans * m_data.QuadVertexPositions[i];
			m_data.BufferPtr->Color = color;
			m_data.BufferPtr->TexCoords = m_default_tex_coords[i];
			m_data.BufferPtr->TexIndex = texture_index;
			m_data.BufferPtr->MiddlePoint = glm::vec2(BASE_XPOS(trans), BASE_YPOS(trans));
			m_data.BufferPtr->Rad_Fill_Th = { radius,fill,thick };
			m_data.BufferPtr++;
		}
		m_data.IndexCount += 6;
	}

	void CircleRender::DrawCircle(const glm::mat4& transform, float_t radius, float fill, float thick, const SubTexture& sub_texture, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		if (m_data.IndexCount >= MaxIndexCount || m_data.TextureSlotIndex > MaxTexture - 1)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		const glm::vec2* coords = sub_texture.GetTexCoords();
		BASE_CORE_ASSERT(coords, "GetTexCoords return nullptr");

		int8_t texture_index = 0;
		if (sub_texture.GetId())
			for (int8_t i = 1; i < m_data.TextureSlotIndex; i++)
			{
				if (m_data.TextureSlots[i] == sub_texture.GetId())
				{
					texture_index = i;
					break;
				}
			}

		if (sub_texture.GetId())
			if (!texture_index)
			{
				texture_index = m_data.TextureSlotIndex;
				m_data.TextureSlots[m_data.TextureSlotIndex] = sub_texture.GetId();
				m_data.TextureSlotIndex++;
			}

		glm::mat4 trans = transform;
		if (rotation)
			trans = glm::rotate(transform, glm::radians(rotation), axis);

		for (size_t i = 0; i < m_data.VerticesNumber; i++)
		{
			m_data.BufferPtr->Position = trans * m_data.QuadVertexPositions[i];
			m_data.BufferPtr->Color = color;
			m_data.BufferPtr->TexCoords = coords[i];
			m_data.BufferPtr->TexIndex = texture_index;
			m_data.BufferPtr->MiddlePoint = glm::vec2(BASE_XPOS(trans), BASE_YPOS(trans));
			m_data.BufferPtr->Rad_Fill_Th = { radius,fill,thick };
			m_data.BufferPtr++;
		}
		m_data.IndexCount += 6;
	}
}
}