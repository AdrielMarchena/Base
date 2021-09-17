#pragma once

#include "Base/Base.h"

#include <stdint.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "RenderDatas.h"

#include "gl/glew.h"

#include "utils/gl_error_macro_db.h"
#include "render/Colors.h"
#include "render/Camera.h"
#include "render/Texture.h"
namespace Base
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
	class Render2D;
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
			//mShader->SetUniformMat4f("u_Transform", transform_g);
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
			if (!m_data.IndexCount)
				return;
			mShader->Bind();
			//Current position - first position
			GLsizeiptr size = (uint8_t*)m_data.BufferPtr - (uint8_t*)m_data.Buffer;

			//TODO: seek a way to sort this jumping the vertices of the same Quad
			// example in case of a Quad, advance 4 Vertex ahead instead of one
			std::sort(m_data.Buffer, m_data.BufferPtr, [&](Vertex* buffer_a, Vertex* buffer_b) 
			{
				return buffer_a->Position.z > buffer_b->Position.z;
			}); //TODO: move to static render

			m_data.VB.Bind();
			m_data.VB.SubData(size, m_data.Buffer);
		};

		virtual void Flush()
		{
			if (!m_data.IndexCount)
				return;
			mShader->Bind();
			m_data.VA.Bind();
			GLCall(glDrawElements(m_data.Target, m_data.IndexCount, GL_UNSIGNED_INT, nullptr));
			m_data.RenderStatus.DrawCount++;

			m_data.Count = 0;
			m_data.IndexCount = 0;
		};

		virtual void Dispose()
		{
			mShader->Dispose();
			m_data.VA.Dispose();
			m_data.VB.Dispose();
			m_data.IB.Dispose();
			WhiteTexture()->Dispose();
			delete[] m_data.Buffer;
			disposed = true;
		};

		void SampleTex(int32_t MaxTexture)
		{
			int32_t* samplers = new int32_t[MaxTexture];
			for (int i = 0; i < MaxTexture; i++)
				samplers[i] = i;
			mShader->SetUniform1iv("u_Textures", MaxTexture, samplers);
			delete[] samplers;

			m_data.TextureSlots = std::vector<uint32_t>(MaxTexture);
			m_data.TextureSlots[0] = WhiteTexture()->GetId();
			for (size_t i = 1; i < MaxTexture; i++)
				m_data.TextureSlots[i] = 0;
		}

		virtual const Ref<Shader> GetShader() { return mShader; };
		virtual const void SetShader(const Ref<Shader>& shader) { mShader = shader; }
 
		virtual T& GetData() { return m_data; }

		inline glm::mat4 pos_trans(const glm::vec3& pos, const glm::vec2& size)
		{
			return glm::translate(glm::mat4(1.0f), pos)
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		}

	protected:
		T m_data;
		Ref<Shader> mShader;
		static Ref<Texture> WhiteTexture() 
		{ 
			static Ref<Texture> white_texture = std::make_shared<Texture>(0xffffffff, 1, 1); 
			return white_texture;
		};
		bool disposed = false;
		friend Render2D;
	};
}
}