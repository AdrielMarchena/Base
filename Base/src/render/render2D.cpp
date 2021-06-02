/*
*	@file render2D.cpp
*	
*	The render it self it's here (cpp)
*
*	Implementation File
*/

#include "render2D.h"
#include "glm/gtc/matrix_transform.hpp"
#include "gl/glew.h"
#include <iostream>
namespace en
{
namespace render
{
	glm::vec3 render2D::m_default_axis = { 0.0f,0.0f,1.0f };
	glm::vec2 render2D::m_default_tex_coords[] = {
		{0.0f,0.0f},
		{1.0f,0.0f},
		{1.0f,1.0f},
		{0.0f,1.0f}
	};

	static inline int32_t MaxTexturesSlots() 
	{
		int32_t MaxT = 8;
		glGetIntegerv(GL_MAX_TEXTURE_UNITS, &MaxT);
		return MaxT;
	}

	static const size_t MaxQuadCount = 3000;
	static const size_t MaxVertexCount = MaxQuadCount * 4;
	static const size_t MaxIndexCount = MaxQuadCount * 6;
	static int32_t MaxTexture = MaxTexturesSlots();

	render2D::render2D(const char* vs, const char* fs)
		:m_data(vs,fs,MaxTexture)
	{
		m_data.Shader.Bind();

		m_data.QuadBuffer = new Vertex[MaxVertexCount];

		glGenVertexArrays(1, &m_data.QuadVA);
		glBindVertexArray(m_data.QuadVA);

		glGenBuffers(1, &m_data.QuadVB);
		glBindBuffer(GL_ARRAY_BUFFER, m_data.QuadVB);
		glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexIndex));

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

		glGenBuffers(1, &m_data.QuadIB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data.QuadIB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _msize(indices), indices, GL_STATIC_DRAW);
		delete[] indices;

		//1x1 white Texture
		glGenTextures(1, &m_data.WhiteTexture);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_data.WhiteTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		uint32_t color = 0xffffffff;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_MULTISAMPLE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		int32_t* samplers = new int32_t[MaxTexture];
		for (int i = 0; i < MaxTexture; i++)
			samplers[i] = i;
		m_data.Shader.SetUniform1iv("u_Textures", MaxTexture, samplers);
		delete[] samplers;

		m_data.TextureSlots = std::vector<uint32_t>(MaxTexture);
		m_data.TextureSlots[0] = m_data.WhiteTexture;
		for (size_t i = 1; i < MaxTexture; i++)
			m_data.TextureSlots[i] = 0;

	}

	render2D::~render2D()
	{
		glDeleteVertexArrays(1, &m_data.QuadVA);
		glDeleteBuffers(1, &m_data.QuadVB);
		glDeleteBuffers(1, &m_data.QuadIB);
		glDeleteTextures(1, &m_data.WhiteTexture);
		delete[] m_data.QuadBuffer;
	}
	
	void render2D::BeginBatch()
	{
		m_data.QuadBufferPtr = m_data.QuadBuffer;
	}

	void render2D::EndBatch()
	{
		//Current position - first position
		GLsizeiptr size = (uint8_t*)m_data.QuadBufferPtr - (uint8_t*)m_data.QuadBuffer;
		glBindBuffer(GL_ARRAY_BUFFER, m_data.QuadVB);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_data.QuadBuffer);
	}

	void render2D::Flush()
	{
		for (size_t i = 0 ; i < m_data.TextureSlotIndex; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_data.TextureSlots[i]);
		}

		glBindVertexArray(m_data.QuadVA);
		glDrawElements(GL_TRIANGLES, m_data.IndexCount, GL_UNSIGNED_INT, nullptr);
		m_data.RenderStatus.DrawCount++;

		m_data.IndexCount = 0;
		m_data.TextureSlotIndex = 1;
	}

	const Shader& render2D::GetShader()
	{
		return m_data.Shader;
	}

	void render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float_t layer, float_t rotation, const glm::vec3& axis)
	{
		if (m_data.IndexCount >= MaxIndexCount)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}
		int32_t texture_index = 0.0f;

		FillV(position, size, color, m_default_tex_coords, texture_index, layer, rotation, axis);
		m_data.IndexCount += 6;
		m_data.RenderStatus.QuadCount++;
	}

	void render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color[4], float_t layer, float_t rotation, const glm::vec3& axis)
	{
		if (m_data.IndexCount >= MaxIndexCount)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}
		int32_t texture_index = 0.0f;

		FillVC(position, size, color, m_default_tex_coords, texture_index, layer, rotation, axis);
		m_data.IndexCount += 6;
		m_data.RenderStatus.QuadCount++;
	}

	void render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture& texture, float_t layer, float_t rotation, const glm::vec3& axis)
	{
		if (m_data.IndexCount >= MaxIndexCount || m_data.TextureSlotIndex > MaxTexture - 1)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		constexpr glm::vec4 color = { 1.0f,1.0f ,1.0f ,1.0f };

		int32_t texture_index = 0.0f;
		if(texture.GetId())
			for (uint32_t i = 1; i < m_data.TextureSlotIndex; i++)
			{
				if (m_data.TextureSlots[i] == texture.GetId())
				{
					texture_index = (float)i;
					break;
				}
			}
		
		if (texture.GetId())
			if (!texture_index)
			{
				texture_index = (float)m_data.TextureSlotIndex;
				m_data.TextureSlots[m_data.TextureSlotIndex] = texture.GetId();
				m_data.TextureSlotIndex++;
			}

		FillV(position, size, color, m_default_tex_coords, texture_index, layer, rotation, axis);
		
		m_data.IndexCount += 6;
		m_data.RenderStatus.QuadCount++;
	}

	void render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const SubTexture& sub_texture, float_t layer, float_t rotation, const glm::vec3& axis)
	{
		if (m_data.IndexCount >= MaxIndexCount || m_data.TextureSlotIndex > MaxTexture - 1)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		constexpr glm::vec4 color = { 1.0f,1.0f ,1.0f ,1.0f };

		int32_t texture_index = 0.0f;
		if (sub_texture.GetId())
			for (uint32_t i = 1; i < m_data.TextureSlotIndex; i++)
			{
				if (m_data.TextureSlots[i] == sub_texture.GetId())
				{
					texture_index = (float)i;
					break;
				}
			}

		if (sub_texture.GetId())
			if (!texture_index)
			{
				texture_index = (float)m_data.TextureSlotIndex;
				m_data.TextureSlots[m_data.TextureSlotIndex] = sub_texture.GetId();
				m_data.TextureSlotIndex++;
			}

		FillV(position, size, color, sub_texture.GetTexCoords(), texture_index, layer, rotation, axis);
		m_data.IndexCount += 6;
		m_data.RenderStatus.QuadCount++;
	}

	void render2D::DrawOutLineQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float_t thick,
				   float_t layer, float_t rotation, const glm::vec3& axis)
	{
		//FIXME: Bottom and TOP are less thick than Right and Left lines
		//Probably the bottom thick is to the wrong side, and the top as well
		//Or, the Left and Right Lines are to thick
		DrawQuad(position, { thick,size.y }, color,layer,rotation,axis);
		DrawQuad({ position.x,position.y + size.y }, { size.x,thick }, color, layer, rotation, axis);
		DrawQuad(position + size, { -thick,-size.y }, color, layer, rotation, axis);
		DrawQuad({ position.x + size.x,position.y }, { -size.x,-thick }, color, layer, rotation, axis);
	}

	void render2D::DrawLine(const glm::vec2& origin, const glm::vec2& dest, const glm::vec4& color, float_t thick, float_t layer)
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
			m_data.QuadBufferPtr->Position = { origin.x, origin.y, -layer };
			m_data.QuadBufferPtr->Color = color;
			m_data.QuadBufferPtr->TexCoords = m_default_tex_coords[0];
			m_data.QuadBufferPtr->TexIndex = 0;
			m_data.QuadBufferPtr++;

			m_data.QuadBufferPtr->Position = { origin.x, origin.y + thick / 2, -layer };
			m_data.QuadBufferPtr->Color = color;
			m_data.QuadBufferPtr->TexCoords = m_default_tex_coords[1];
			m_data.QuadBufferPtr->TexIndex = 0;
			m_data.QuadBufferPtr++;

			m_data.QuadBufferPtr->Position = { dest.x, dest.y, -layer };
			m_data.QuadBufferPtr->Color = color;
			m_data.QuadBufferPtr->TexCoords = m_default_tex_coords[2];
			m_data.QuadBufferPtr->TexIndex = 0;
			m_data.QuadBufferPtr++;

			m_data.QuadBufferPtr->Position = { dest.x, dest.y - thick / 2, -layer };
			m_data.QuadBufferPtr->Color = color;
			m_data.QuadBufferPtr->TexCoords = m_default_tex_coords[3];
			m_data.QuadBufferPtr->TexIndex = 0;
			m_data.QuadBufferPtr++;

			m_data.IndexCount += 6;
		}
		else
		if ((origin.x < dest.x && origin.y < dest.y) || (origin.x > dest.x && origin.y > dest.y))
		{
			m_data.QuadBufferPtr->Position = { dest.x, dest.y, -layer };
			m_data.QuadBufferPtr->Color = color;
			m_data.QuadBufferPtr->TexCoords = m_default_tex_coords[0];
			m_data.QuadBufferPtr->TexIndex = 0;
			m_data.QuadBufferPtr++;

			m_data.QuadBufferPtr->Position = { dest.x, dest.y + thick / 2, -layer };
			m_data.QuadBufferPtr->Color = color;
			m_data.QuadBufferPtr->TexCoords = m_default_tex_coords[1];
			m_data.QuadBufferPtr->TexIndex = 0;
			m_data.QuadBufferPtr++;

			m_data.QuadBufferPtr->Position = { origin.x, origin.y, -layer };
			m_data.QuadBufferPtr->Color = color;
			m_data.QuadBufferPtr->TexCoords = m_default_tex_coords[2];
			m_data.QuadBufferPtr->TexIndex = 0;
			m_data.QuadBufferPtr++;

			m_data.QuadBufferPtr->Position = { origin.x, origin.y - thick / 2, -layer };
			m_data.QuadBufferPtr->Color = color;
			m_data.QuadBufferPtr->TexCoords = m_default_tex_coords[3];
			m_data.QuadBufferPtr->TexIndex = 0;
			m_data.QuadBufferPtr++;

			m_data.IndexCount += 6;
		}
	}

	void render2D::rotate(glm::vec3 vertices[4], float rotation,const glm::vec3& rotationCenter,const glm::vec3& axis)
	{
		const glm::mat4 translationMatrix = glm::translate(glm::identity<glm::mat4>(), -rotationCenter);
		const glm::mat4 rotationMatrix = glm::rotate(glm::identity<glm::mat4>(), rotation, axis);
		const glm::mat4 reverseTranslationMatrix = glm::translate(glm::identity<glm::mat4>(), rotationCenter);

		for (size_t i = 0; i < 4; i++) {
			vertices[i] = glm::vec3(
				reverseTranslationMatrix * rotationMatrix * translationMatrix * glm::vec4(vertices[i], 1.0f));
		}
	}

	void render2D::FillV(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const glm::vec2 tex_coords[4], float_t tex_index, float_t layer, float_t rotation, const glm::vec3& axis)
	{
		glm::vec3 quads[4] = {
			{position.x,position.y,-layer},
			{position.x + size.x, position.y,-layer},
			{position.x + size.x,position.y + size.y,-layer},
			{position.x,position.y + size.y,-layer}
		};

		if (rotation)
			rotate(quads, rotation, { position.x + (size.x / 2), position.y + (size.y / 2), layer }, axis);

		constexpr glm::vec3 LightPos = { 400.0f,300.0f,0.0f };
		constexpr glm::vec4 LightColor = { 1.0f,1.0f,1.0f,1.0f };
	
		constexpr glm::vec2 LightNormals[4] =
		{
			{1.0f,1.0f},
			{1.0f,1.0f},
			{1.0f,1.0f},
			{1.0f,1.0f},
		};

		for (uint8_t i = 0; i < 4; i++)
		{
			m_data.QuadBufferPtr->Position = quads[i];
			m_data.QuadBufferPtr->Color = color;
			m_data.QuadBufferPtr->TexCoords = tex_coords[i];
			m_data.QuadBufferPtr->TexIndex = tex_index;
			m_data.QuadBufferPtr++;
		}
	}
	void render2D::FillVC(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color[4], const glm::vec2 tex_coords[4], float_t tex_index, float_t layer, float_t rotation, const glm::vec3& axis)
	{
		glm::vec3 quads[4] = {
			{position.x,position.y,-layer},
			{position.x + size.x, position.y,-layer},
			{position.x + size.x,position.y + size.y,-layer},
			{position.x,position.y + size.y,-layer}
		};

		if (rotation)
			rotate(quads, rotation, { position.x + (size.x / 2), position.y + (size.y / 2), layer }, axis);

		for (uint8_t i = 0; i < 4; i++)
		{
			m_data.QuadBufferPtr->Position = quads[i];
			m_data.QuadBufferPtr->Color = color[i];
			m_data.QuadBufferPtr->TexCoords = tex_coords[i];
			m_data.QuadBufferPtr->TexIndex = tex_index;
			m_data.QuadBufferPtr++;
		}
	}
}
}