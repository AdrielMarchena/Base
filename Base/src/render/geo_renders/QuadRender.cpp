/*
*	@file QuadRender2D.cpp
*
*	The render it self it's here (cpp)
*
*	Implementation File
*/

#include "QuadRender.h"
#include "glm/gtc/matrix_transform.hpp"
#include "gl/glew.h"
#include <iostream>
namespace Base
{
	namespace render
	{

		glm::vec2 QuadRender2D::m_default_tex_coords[] = {
		{0.0f,0.0f},
		{1.0f,0.0f},
		{1.0f,1.0f},
		{0.0f,1.0f}
		};

		static const size_t MaxQuadCount = 1000;
		static const size_t MaxVertexCount = MaxQuadCount * 4;
		static const size_t MaxIndexCount = MaxQuadCount * 6;
		static int32_t MaxTexture = MaxTexturesSlots();

		QuadRender2D::QuadRender2D(const char* vs, const char* fs)
		{
			mShader = std::move(Shader(vs, fs, MaxTexturesSlots()));
			mShader = Shader::CreateShader(vs, fs, MaxTexturesSlots());
			mShader.Bind();

			m_data.Buffer = new QuadVertex[MaxVertexCount];

			//GLCall(glGenVertexArrays(1, &m_data.VA));
			//GLCall(glBindVertexArray(m_data.VA));
			m_data.VA = VertexArray::CreateVertexArray();

			//GLCall(glGenBuffers(1, &m_data.VB));
			//GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_data.VB));
			//GLCall(glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(QuadVertex), nullptr, GL_DYNAMIC_DRAW));
			m_data.VB = VertexBuffer::CreateVertexBuffer(MaxVertexCount * sizeof(QuadVertex));

			VertexAttribute layout(m_data.VA, m_data.VB);

			//GLCall(glEnableVertexAttribArray(0));
			//GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, Position)));
			layout.AddLayout<float>(3, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, Position));

			//GLCall(glEnableVertexAttribArray(1));
			//GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, Color)));
			layout.AddLayout<float>(4, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, Color));

			//GLCall(glEnableVertexAttribArray(2));
			//GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, TexCoords)));
			layout.AddLayout<float>(2, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, TexCoords));

			//GLCall(glEnableVertexAttribArray(3));
			//GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, TexIndex)));
			layout.AddLayout<float>(1, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, TexIndex));

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

			//GLCall(glGenBuffers(1, &m_data.IB));
			//GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data.IB));
			//GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _msize(indices), indices, GL_STATIC_DRAW));
			m_data.IB = IndexBuffer::CreateIndexBuffer(_msize(indices), indices);
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
			mShader.SetUniform1iv("u_Textures", MaxTexture, samplers);
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

		void QuadRender2D::Flush()
		{
			mShader.Bind();
			for (uint8_t i = 0; i < m_data.TextureSlotIndex; i++)
			{
				GLCall(glActiveTexture(GL_TEXTURE0 + i));
				GLCall(glBindTexture(GL_TEXTURE_2D, m_data.TextureSlots[i]));
			}
			Render::Flush();
			m_data.TextureSlotIndex = 1;
		}

		void QuadRender2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,  float_t rotation, const glm::vec3& axis)
		{
			DrawQuad(pos_trans(position,size), color);
		}

		void QuadRender2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color[4], float_t rotation, const glm::vec3& axis)
		{
			DrawQuad(pos_trans(position, size), color);
		}

		void QuadRender2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Texture& texture, const glm::vec4 & color, float_t rotation, const glm::vec3& axis)
		{
			DrawQuad(pos_trans(position,size), texture, color, rotation, axis);
		}

		void QuadRender2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const SubTexture& sub_texture, const glm::vec4& color,float_t rotation, const glm::vec3& axis)
		{
			DrawQuad(pos_trans(position, size), sub_texture, color, rotation, axis);
		}

		void QuadRender2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
		{
			glm::vec4 colors[4]{};
			for (uint8_t i = 0; i < 4; i++) //TODO: Necessary? Idk
				colors[i] = color;
			DrawQuad(transform, colors, rotation, axis);
		}

		void QuadRender2D::DrawQuad(const glm::mat4& transform, const glm::vec4 color[4], float_t rotation, const glm::vec3& axis)
		{
			if (m_data.IndexCount >= MaxIndexCount)
			{
				EndBatch();
				Flush();
				BeginBatch();
			}

			glm::mat4 trans = transform;
			if (rotation)
				trans = glm::rotate(transform, glm::radians(rotation), axis);

			for (size_t i = 0; i < 4; i++)
			{
				m_data.BufferPtr->Position = trans * m_data.QuadVertexPositions[i];
				m_data.BufferPtr->Color = color[i];
				m_data.BufferPtr->TexCoords = m_default_tex_coords[i];
				m_data.BufferPtr->TexIndex = m_data.WhiteTextureSlot;
				m_data.BufferPtr++;
			}
			m_data.IndexCount += 6;
			m_data.RenderStatus.QuadCount++;
		}

		void QuadRender2D::DrawQuad(const glm::mat4& transform, const Texture& texture, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
		{
			if (m_data.IndexCount >= MaxIndexCount || m_data.TextureSlotIndex > MaxTexture - 1)
			{
				EndBatch();
				Flush();
				BeginBatch();
			}

			int8_t texture_index = 0;
			if (texture.GetId())
				for (int8_t i = 1; i < m_data.TextureSlotIndex; i++)
				{
					if (m_data.TextureSlots[i] == texture.GetId())
					{
						texture_index = i;
						break;
					}
				}

			if (texture.GetId())
				if (!texture_index)
				{
					texture_index = m_data.TextureSlotIndex;
					m_data.TextureSlots[m_data.TextureSlotIndex] = texture.GetId();
					m_data.TextureSlotIndex++;
				}

			glm::mat4 trans = transform;
			if (rotation)
				trans = glm::rotate(transform, glm::radians(rotation), axis);

			for (size_t i = 0; i < 4; i++)
			{
				m_data.BufferPtr->Position = trans * m_data.QuadVertexPositions[i];
				m_data.BufferPtr->Color = color;
				m_data.BufferPtr->TexCoords = m_default_tex_coords[i];
				m_data.BufferPtr->TexIndex = texture_index;
				m_data.BufferPtr++;
			}

			m_data.IndexCount += 6;
			m_data.RenderStatus.QuadCount++;
		}

		void QuadRender2D::DrawQuad(const glm::mat4& transform, const SubTexture& sub_texture, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
		{
			if (m_data.IndexCount >= MaxIndexCount || m_data.TextureSlotIndex > MaxTexture - 1)
			{
				EndBatch();
				Flush();
				BeginBatch();
			}

			auto coords = sub_texture.GetTexCoords();

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

			for (size_t i = 0; i < 4; i++)
			{
				m_data.BufferPtr->Position = trans * m_data.QuadVertexPositions[i];
				m_data.BufferPtr->Color = color;
				m_data.BufferPtr->TexCoords = coords[i];
				m_data.BufferPtr->TexIndex = texture_index;
				m_data.BufferPtr++;
			}

			m_data.IndexCount += 6;
			m_data.RenderStatus.QuadCount++;
		}

		void QuadRender2D::DrawQuadLine(const glm::vec2& origin, const glm::vec2& dest, const glm::vec4& color, float_t thick, float_t layer)
		{
			if (m_data.IndexCount >= MaxIndexCount)
			{
				EndBatch();
				Flush();
				BeginBatch();
			}

			//RIGHT UP LEFT DOWN

			//This check make the line make more sense (kinda of)
			if ((origin.x > dest.x && origin.y < dest.y) || (origin.x < dest.x && origin.y > dest.y))
			{
				m_data.BufferPtr->Position = { origin.x, origin.y, -layer };
				m_data.BufferPtr->Color = color;
				m_data.BufferPtr->TexCoords = m_default_tex_coords[0];
				m_data.BufferPtr->TexIndex = 0;
				m_data.BufferPtr++;

				m_data.BufferPtr->Position = { origin.x, origin.y + thick / 2, -layer };
				m_data.BufferPtr->Color = color;
				m_data.BufferPtr->TexCoords = m_default_tex_coords[1];
				m_data.BufferPtr->TexIndex = 0;
				m_data.BufferPtr++;

				m_data.BufferPtr->Position = { dest.x, dest.y, -layer };
				m_data.BufferPtr->Color = color;
				m_data.BufferPtr->TexCoords = m_default_tex_coords[2];
				m_data.BufferPtr->TexIndex = 0;
				m_data.BufferPtr++;

				m_data.BufferPtr->Position = { dest.x, dest.y - thick / 2, -layer };
				m_data.BufferPtr->Color = color;
				m_data.BufferPtr->TexCoords = m_default_tex_coords[3];
				m_data.BufferPtr->TexIndex = 0;
				m_data.BufferPtr++;

				m_data.IndexCount += 6;
			}
			else
				if ((origin.x < dest.x && origin.y < dest.y) || (origin.x > dest.x && origin.y > dest.y))
				{
					m_data.BufferPtr->Position = { dest.x, dest.y, -layer };
					m_data.BufferPtr->Color = color;
					m_data.BufferPtr->TexCoords = m_default_tex_coords[0];
					m_data.BufferPtr->TexIndex = 0;
					m_data.BufferPtr++;

					m_data.BufferPtr->Position = { dest.x, dest.y + thick / 2, -layer };
					m_data.BufferPtr->Color = color;
					m_data.BufferPtr->TexCoords = m_default_tex_coords[1];
					m_data.BufferPtr->TexIndex = 0;
					m_data.BufferPtr++;

					m_data.BufferPtr->Position = { origin.x, origin.y, -layer };
					m_data.BufferPtr->Color = color;
					m_data.BufferPtr->TexCoords = m_default_tex_coords[2];
					m_data.BufferPtr->TexIndex = 0;
					m_data.BufferPtr++;

					m_data.BufferPtr->Position = { origin.x, origin.y - thick / 2, -layer };
					m_data.BufferPtr->Color = color;
					m_data.BufferPtr->TexCoords = m_default_tex_coords[3];
					m_data.BufferPtr->TexIndex = 0;
					m_data.BufferPtr++;

					m_data.IndexCount += 6;
				}
		}

	}
}