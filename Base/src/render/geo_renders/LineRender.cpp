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
#include "utils/gl_error_macro_db.h"
namespace en
{
	namespace render
	{

		static int32_t MaxTexture = Render::MaxTexturesSlots();
		static const size_t MaxLineCount = 1000;
		static const size_t MaxLineVertexCount = MaxLineCount * 2;
		static const size_t MaxLineIndexCount = MaxLineCount * 2;

		LineRender2D::LineRender2D(const char* vs, const char* fs)
			:m_data(vs, fs, MaxTexture)
		{
			//Line Stuff
			m_data.mShader.Bind();

			m_data.Buffer = new LineVertex[MaxLineVertexCount];

			GLCall(glGenVertexArrays(1, &m_data.VA));
			GLCall(glBindVertexArray(m_data.VA));

			GLCall(glGenBuffers(1, &m_data.VB));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_data.VB));
			GLCall(glBufferData(GL_ARRAY_BUFFER, MaxLineVertexCount * sizeof(LineVertex), nullptr, GL_DYNAMIC_DRAW));

			GLCall(glEnableVertexAttribArray(0));
			GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (const void*)offsetof(LineVertex, Position)));

			GLCall(glEnableVertexAttribArray(1));
			GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (const void*)offsetof(LineVertex, Color)));

			//To much memory for the stack, free below
			uint32_t* indices = new uint32_t[MaxLineIndexCount]{};
			uint32_t offset = 0;
			for (int i = 0; i < MaxLineIndexCount; i += 2)
			{
				indices[i + 0] = 0 + offset;
				indices[i + 1] = 1 + offset;

				offset += 2;
			}

			GLCall(glGenBuffers(1, &m_data.IB));
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data.IB));
			GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _msize(indices), indices, GL_STATIC_DRAW));
			delete[] indices;

		}

		LineRender2D::~LineRender2D()
		{
			GLCall(glDeleteVertexArrays(1, &m_data.VA));
			GLCall(glDeleteBuffers(1, &m_data.VB));
			GLCall(glDeleteBuffers(1, &m_data.IB));
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
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_data.VB));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_data.Buffer));
		}

		void LineRender2D::Flush()
		{
			m_data.mShader.Bind();
			GLCall(glBindVertexArray(m_data.VA));
			GLCall(glDrawElements(GL_LINES, m_data.IndexCount, GL_UNSIGNED_INT, nullptr));
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

		//Find this here
		//https://www.programmersought.com/article/74894584653/
		static inline glm::vec2 bezier_3order_mix(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, float_t t)
		{
			const glm::vec2 q0 = glm::mix(p0, p1, t);
			const glm::vec2 q1 = glm::mix(p1, p2, t);
			const glm::vec2 q2 = glm::mix(p2, p3, t);
			
			const glm::vec2 r0 = glm::mix(q0, q1, t);
			const glm::vec2 r1 = glm::mix(q1, q2, t);

			return glm::mix(r0, r1, t);
		}

		static inline glm::vec2 bezier_2order_mix(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, float_t t)
		{
			const glm::vec2 q0 = glm::mix(p0, p1, t);
			const glm::vec2 q1 = glm::mix(p1, p2, t);

			return glm::mix(q0, q1, t);
		}

		void LineRender2D::DrawCurveLine(const glm::vec2& origin, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& dest, const glm::vec4& color,float_t precision ,float_t layer)
		{
			std::vector<glm::vec2> points;
			for (float i = 0; i < 1; i+= precision)
			{
				//Remove this loop
				points.push_back(bezier_3order_mix(origin, p1, p2, dest,i));
			}

			for (int i = 0; i < points.size()-1; i++)
			{
				DrawLine(points[i], points[i+1], color, layer);
			}
			DrawLine(points.back(), dest,color,layer); // Connect the last line with the destination
		}
		void LineRender2D::DrawCurveLine(const glm::vec2& origin, const glm::vec2& p1, const glm::vec2& dest, const glm::vec4& color, float_t precision, float_t layer)
		{
			std::vector<glm::vec2> points;
			for (float i = 0; i < 1; i += precision)
			{
				//Remove this loop
				points.push_back(bezier_2order_mix(origin, p1, dest, i));
			}

			for (int i = 0; i < points.size() - 1; i++)
			{
				DrawLine(points[i], points[i + 1], color, layer);
			}
			DrawLine(points.back(), dest, color, layer); // Connect the last line with the destination
		}
	}
}