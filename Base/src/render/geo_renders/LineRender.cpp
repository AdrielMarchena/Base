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
		static const size_t MaxLineCount = 100;
		static const size_t MaxLineVertexCount = MaxLineCount * 2;
		static const size_t MaxLineIndexCount = MaxLineCount * 2;

		LineRender2D::LineRender2D(const Ref<Shader>& shader)
		{
			mShader = shader;
			mShader->Bind();
			m_data.Target = GL_Target::LINES;
			m_data.Buffer = new LineVertex[MaxLineVertexCount];
			
			m_data.VerticesNumber = 2;

			m_data.VA = VertexArray::CreateVertexArray();

			m_data.VB = VertexBuffer::CreateVertexBuffer(MaxLineVertexCount * sizeof(LineVertex));

			VertexAttribute layout(m_data.VB);
			layout.AddLayoutFloat(3, sizeof(LineVertex), (const void*)offsetof(LineVertex, Position));

			layout.AddLayoutFloat(4, sizeof(LineVertex), (const void*)offsetof(LineVertex, Color));

		}

		void LineRender2D::Flush()
		{
			if (!m_data.IndexCount)
				return;
			m_data.VA->Bind();
			GLCommands::GL_DrawArrayCall(m_data.Target, 0, m_data.IndexCount);
			//GLCall(glDrawElements(m_data.Target, m_data.IndexCount, GL_UNSIGNED_INT, nullptr));
			m_data.RenderStatus.DrawCount++;

			m_data.Count = 0;
			m_data.IndexCount = 0;
		}

		void LineRender2D::DrawLine(const glm::vec3& origin, const glm::vec3& dest, const glm::vec4& color)
		{
			if (m_data.IndexCount >= MaxLineIndexCount)
			{
				EndBatch();
				Flush();
				BeginBatch();
			}

			m_data.BufferPtr->Position = origin;
			m_data.BufferPtr->Color = color;
			m_data.BufferPtr++;

			m_data.BufferPtr->Position = dest;
			m_data.BufferPtr->Color = color;
			m_data.BufferPtr++;

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

		void LineRender2D::DrawQuadLine(const glm::mat4& transform, const glm::vec4& color)
		{
			static constexpr glm::vec4 QuadVertexPositions[] =
			{
				{ -0.5f, -0.5f, 0.0f, 1.0f },
				{ 0.5f, -0.5f, 0.0f, 1.0f },
				{ 0.5f,  0.5f, 0.0f, 1.0f },
				{ -0.5f,  0.5f, 0.0f, 1.0f }
			};

			glm::vec3 quadLines[4];
			for (int i = 0; i < 4; i++)
				quadLines[i] = transform * QuadVertexPositions[i];

			DrawLine(quadLines[0], quadLines[1], color);
			DrawLine(quadLines[1], quadLines[2], color);
			DrawLine(quadLines[2], quadLines[3], color);
			DrawLine(quadLines[3], quadLines[0], color);
		}

		void LineRender2D::SetLineWidth(float_t thickness)
		{
			GLCall(glLineWidth(thickness));
		}
	}
}