/*
*	@file LineRender2D.cpp
*
*	The render it self it's here (cpp)
*
*	Implementation File
*/

#include "LineRender.h"
#include "glm/gtc/matrix_transform.hpp"
#include "gl/glew.h"
#include <iostream>
namespace en
{
	namespace render
	{
		static inline int32_t MaxTexturesSlots()
		{
			int32_t MaxT = 8;
			glGetIntegerv(GL_MAX_TEXTURE_UNITS, &MaxT);
			return MaxT;
		}

		static int32_t MaxTexture = MaxTexturesSlots();

		static const size_t MaxLineCount = 3000;
		static const size_t MaxLineVertexCount = MaxLineCount * 2;
		static const size_t MaxLineIndexCount = MaxLineCount * 2;

		LineRender2D::LineRender2D(const char* vs, const char* fs)
			:m_data(vs, fs, MaxTexture)
		{
			//Line Stuff
			m_data.mShader.Bind();

			m_data.Buffer = new LineVertex[MaxLineVertexCount];

			glGenVertexArrays(1, &m_data.VA);
			glBindVertexArray(m_data.VA);

			glGenBuffers(1, &m_data.VB);
			glBindBuffer(GL_ARRAY_BUFFER, m_data.VB);
			glBufferData(GL_ARRAY_BUFFER, MaxLineVertexCount * sizeof(LineVertex), nullptr, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (const void*)offsetof(LineVertex, Position));

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (const void*)offsetof(LineVertex, Color));

			//To much memory for the stack, free below
			uint32_t* indices = new uint32_t[MaxLineIndexCount]{};
			uint32_t offset = 0;
			for (int i = 0; i < MaxLineIndexCount; i += 2)
			{
				indices[i + 0] = 0 + offset;
				indices[i + 1] = 1 + offset;

				offset += 2;
			}

			glGenBuffers(1, &m_data.IB);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data.IB);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _msize(indices), indices, GL_STATIC_DRAW);
			delete[] indices;

		}

		LineRender2D::~LineRender2D()
		{
			glDeleteVertexArrays(1, &m_data.VA);
			glDeleteBuffers(1, &m_data.VB);
			glDeleteBuffers(1, &m_data.IB);
			delete[] m_data.Buffer;
		}

		void LineRender2D::BeginBatch()
		{
			m_data.BufferPtr = m_data.Buffer;
		}

		void LineRender2D::EndBatch()
		{
			m_data.mShader.Bind();
			//Current position - first position
			GLsizeiptr size = (uint8_t*)m_data.BufferPtr - (uint8_t*)m_data.Buffer;
			glBindBuffer(GL_ARRAY_BUFFER, m_data.VB);
			glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_data.Buffer);
		}

		void LineRender2D::Flush()
		{
			m_data.mShader.Bind();
			glBindVertexArray(m_data.VA);
			//glBindBuffer(GL_ARRAY_BUFFER, m_data.LineVB);
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data.LineIB);
			glDrawElements(GL_LINES, m_data.IndexCount, GL_UNSIGNED_INT, nullptr);
			m_data.RenderStatus.DrawCount++;

			m_data.Count = 0;
			m_data.IndexCount = 0;
		}

		const Shader& LineRender2D::GetShader()
		{
			return m_data.mShader;
		}

		void LineRender2D::DrawLine(const glm::vec2& origin, const glm::vec2& dest, const glm::vec4& color, float_t layer)
		{
			if (m_data.IndexCount >= MaxLineIndexCount)
			{
				EndBatch();
				Flush();
				BeginBatch();
			}

			m_data.BufferPtr->Position = { origin.x,origin.y,-layer };
			m_data.BufferPtr->Color = color;
			m_data.BufferPtr++;

			m_data.BufferPtr->Position = { dest.x,dest.y,-layer };
			m_data.BufferPtr->Color = color;
			m_data.BufferPtr++;

			m_data.Count++;
			m_data.IndexCount += 2;
		}
	}
}