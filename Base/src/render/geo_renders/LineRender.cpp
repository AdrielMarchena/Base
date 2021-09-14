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
namespace Base
{
	namespace render
	{

		static int32_t MaxTexture = MaxTexturesSlots();
		static const size_t MaxLineCount = 1000;
		static const size_t MaxLineVertexCount = MaxLineCount * 2;
		static const size_t MaxLineIndexCount = MaxLineCount * 2;

		LineRender2D::LineRender2D(const char* vs, const char* fs)
		{
			//Line Stuff
			mShader = Shader::CreateShader(vs, fs, MaxTexturesSlots());
			mShader->Bind();
			m_data.Target = GL_LINES;
			m_data.Buffer = new LineVertex[MaxLineVertexCount];

			//GLCall(glGenVertexArrays(1, &m_data.VA));
			//GLCall(glBindVertexArray(m_data.VA));
			m_data.VA = VertexArray::CreateVertexArray();

			//GLCall(glGenBuffers(1, &m_data.VB));
			//GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_data.VB));
			//GLCall(glBufferData(GL_ARRAY_BUFFER, MaxLineVertexCount * sizeof(LineVertex), nullptr, GL_DYNAMIC_DRAW));
			m_data.VB = VertexBuffer::CreateVertexBuffer(MaxLineVertexCount * sizeof(LineVertex));

			VertexAttribute layout(m_data.VA, m_data.VB);
			//GLCall(glEnableVertexAttribArray(0));
			//GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (const void*)offsetof(LineVertex, Position)));
			layout.AddLayout<float>(3, sizeof(LineVertex), (const void*)offsetof(LineVertex, Position));

			//GLCall(glEnableVertexAttribArray(1));
			//GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (const void*)offsetof(LineVertex, Color)));
			layout.AddLayout<float>(4, sizeof(LineVertex), (const void*)offsetof(LineVertex, Color));

			//To much memory for the stack, free below
			uint32_t* indices = new uint32_t[MaxLineIndexCount]{};
			uint32_t offset = 0;
			for (int i = 0; i < MaxLineIndexCount; i += 2)
			{
				indices[i + 0] = 0 + offset;
				indices[i + 1] = 1 + offset;

				offset += 2;
			}

			//GLCall(glGenBuffers(1, &m_data.IB));
			//GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data.IB));
			//GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _msize(indices), indices, GL_STATIC_DRAW));
			m_data.IB = IndexBuffer::CreateIndexBuffer(_msize(indices), indices);
			delete[] indices;

		}

		void LineRender2D::DrawLine(const glm::vec3& origin, const glm::vec3& dest, const glm::vec4& color)
		{
			if (m_data.IndexCount >= MaxLineIndexCount)
			{
				EndBatch();
				Flush();
				BeginBatch();
			}

			m_data.BufferPtr->Position = { origin.x,origin.y,origin.z };
			m_data.BufferPtr->Color = color;
			m_data.BufferPtr++;

			m_data.BufferPtr->Position = { dest.x,dest.y,origin.z };
			m_data.BufferPtr->Color = color;
			m_data.BufferPtr++;

			m_data.Count++;
			m_data.IndexCount += 2;
		}

		//Find this here
		//https://www.programmersought.com/article/74894584653/
		static inline glm::vec3 bezier_3order_mix(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float_t t)
		{
			const glm::vec3 q0 = glm::mix(p0, p1, t);
			const glm::vec3 q1 = glm::mix(p1, p2, t);
			const glm::vec3 q2 = glm::mix(p2, p3, t);
			
			const glm::vec3 r0 = glm::mix(q0, q1, t);
			const glm::vec3 r1 = glm::mix(q1, q2, t);

			return glm::mix(r0, r1, t);
		}

		static inline glm::vec3 bezier_2order_mix(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, float_t t)
		{
			const glm::vec3 q0 = glm::mix(p0, p1, t);
			const glm::vec3 q1 = glm::mix(p1, p2, t);

			return glm::mix(q0, q1, t);
		}

		void LineRender2D::DrawCurveLine(const glm::vec3& origin, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& dest, const glm::vec4& color,float_t precision)
		{
			glm::vec3* acting_origin = &const_cast<glm::vec3&>(origin);
			glm::vec3 acting_interpol{};
			for (float i = precision; i < 1; i+= precision)
			{
				acting_interpol = bezier_3order_mix(origin, p1, p2, dest, i);
				DrawLine(*acting_origin, acting_interpol, color);
				acting_origin = &acting_interpol;
				//points.push_back(bezier_3order_mix(origin, p1, p2, dest,i));
			}
			DrawLine(*acting_origin, dest,color); // Connect the last line with the destination
		}
		void LineRender2D::DrawCurveLine(const glm::vec3& origin, const glm::vec3& p1, const glm::vec3& dest, const glm::vec4& color, float_t precision)
		{
			glm::vec3* acting_origin = &const_cast<glm::vec3&>(origin);
			glm::vec3 acting_interpol{};
			for (float i = precision; i < 1; i += precision)
			{
				acting_interpol = bezier_2order_mix(origin, p1, dest, i);
				DrawLine(*acting_origin, acting_interpol, color);
				acting_origin = &acting_interpol;
				//points.push_back(bezier_3order_mix(origin, p1, p2, dest,i));
			}
			DrawLine(*acting_origin, dest, color); // Connect the last line with the destination
		}
	}
}