/*
*	@file QuadRender2D.cpp
*
*	The render it self it's here (cpp)
*
*	Implementation File
*/

#include "QuadRender.h"
#include "utils/Generic.h"
#include "glm/gtc/matrix_transform.hpp"
#include "utils/gl_error_macro_db.h"
#include "gl/glew.h"
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

		

		QuadRender2D::QuadRender2D(const Ref<Shader>& shader)
		{
			mShader = shader;
			mShader->Bind();

			m_data.VerticesNumber = 4;

			m_data.Buffer = new QuadVertex[MaxVertexCount];

			m_data.VA = VertexArray::CreateVertexArray();

			m_data.VB = VertexBuffer::CreateVertexBuffer(MaxVertexCount * sizeof(QuadVertex));

			VertexAttribute layout(m_data.VB);

			layout.AddLayoutFloat(3, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, Position));

			layout.AddLayoutFloat(4, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, Color));

			layout.AddLayoutFloat(2, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, TexCoords));

			layout.AddLayoutFloat(1, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, TexIndex));

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
			m_data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
			m_data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
			m_data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		}

		void QuadRender2D::Flush()
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

		void QuadRender2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,  float_t rotation, const glm::vec3& axis)
		{
			DrawQuad(utils::pos_trans(position,size), color);
		}

		void QuadRender2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color[4], float_t rotation, const glm::vec3& axis)
		{
			DrawQuad(utils::pos_trans(position, size), color);
		}

		void QuadRender2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture> texture, const glm::vec4 & color, float_t rotation, const glm::vec3& axis)
		{
			DrawQuad(utils::pos_trans(position,size), texture, color, rotation, axis);
		}

		void QuadRender2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const SubTexture& sub_texture, const glm::vec4& color,float_t rotation, const glm::vec3& axis)
		{
			DrawQuad(utils::pos_trans(position, size), sub_texture, color, rotation, axis);
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

			for (size_t i = 0; i < m_data.VerticesNumber; i++)
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

		void QuadRender2D::DrawQuad(const glm::mat4& transform, Ref<Texture> texture, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
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

			for (size_t i = 0; i < m_data.VerticesNumber; i++)
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