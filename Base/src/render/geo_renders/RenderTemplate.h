#pragma once

#include <stdint.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "RenderDatas.h"

#include "gl/glew.h"

#include "utils/gl_error_macro_db.h"
#include "render/Colors.h"
#include "render/Camera.h"
namespace en
{
namespace render
{

	inline int32_t MaxTexturesSlots()
	{
		static int32_t MaxT = 8;
		//GLCall(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS , &MaxT));
		return MaxT;
	}

	inline void rotate(glm::vec3 vertices[4], float rotation, const glm::vec3& rotationCenter, const glm::vec3& axis)
	{
		const glm::mat4 translationMatrix = glm::translate(glm::identity<glm::mat4>(), -rotationCenter);
		const glm::mat4 rotationMatrix = glm::rotate(glm::identity<glm::mat4>(), rotation, axis);
		const glm::mat4 reverseTranslationMatrix = glm::translate(glm::identity<glm::mat4>(), rotationCenter);

		for (size_t i = 0; i < 4; i++) {
			vertices[i] = glm::vec3(
				reverseTranslationMatrix * rotationMatrix * translationMatrix * glm::vec4(vertices[i], 1.0f));
		}
	}

	template<typename T>
	class Render
	{
	public:
		Render()
			:m_data()
		{ 
		}

		virtual void BeginScene(const glm::mat4& viewProj, const glm::mat4& transform_g)
		{
			mShader->Bind();
			mShader->SetUniformMat4f("u_ViewProj", viewProj);
			mShader->SetUniformMat4f("u_Transform", transform_g);
		}

		virtual void BeginBatch() 
		{ 
			m_data.Count = 0;
			m_data.BufferPtr = m_data.Buffer;
			m_data.IndexCount = 0;
			m_data.TextureSlotIndex = 1;
		};

		virtual void EndBatch()
		{
			mShader->Bind();
			//Current position - first position
			GLsizeiptr size = (uint8_t*)m_data.BufferPtr - (uint8_t*)m_data.Buffer;
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_data.VB));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_data.Buffer));
		};

		virtual void Flush()
		{
			mShader->Bind();
			GLCall(glBindVertexArray(m_data.VA));
			GLCall(glDrawElements(m_data.Target, m_data.IndexCount, GL_UNSIGNED_INT, nullptr));
			m_data.RenderStatus.DrawCount++;

			m_data.Count = 0;
			m_data.IndexCount = 0;
		};

		virtual void Dispose()
		{
			mShader->Dispose();
			GLCall(glDeleteVertexArrays(1, &m_data.VA));
			GLCall(glDeleteBuffers(1, &m_data.VB));
			GLCall(glDeleteBuffers(1, &m_data.IB));
			delete[] m_data.Buffer;
			if (m_data.WhiteTexture)
				GLCall(glDeleteTextures(1, &m_data.WhiteTexture));
			disposed = true;
		};

		virtual const std::shared_ptr<Shader> GetShader() { return mShader; };

		//virtual ~Render() { if (!disposed) Dispose(); }
	
		inline glm::mat4 pos_trans(const glm::vec3& pos, const glm::vec2& size)
		{
			return glm::translate(glm::mat4(1.0f), pos)
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		}
	protected:
		T m_data;
		std::shared_ptr<Shader> mShader;
		bool disposed = false;
	};
}
}