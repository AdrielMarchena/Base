/*
*	@file SubTexture.h
*	
*	Subtexture Stuff is here
*/

#pragma once

#include "Base/Base.h"

#include "gl/Texture.h"
#include "glm/glm.hpp"
namespace Base
{
namespace render
{

	class SubTexture
	{
	private:
		Ref<Texture> m_Texture;
		glm::vec2 m_TexCoords[4] = { glm::vec2(0.0f) };
	public:
		SubTexture() = default;
		SubTexture(Ref<Texture> texture , const glm::vec2& pos, const glm::vec2& size);
		static SubTexture CreateFromCoords(Ref<Texture> texture, const glm::vec2 size, const glm::vec2& coords, const glm::vec2& spriteSize);
		static SubTexture CreateFromAtlas(Ref<Texture> texture, const glm::vec2 size, const glm::vec2& position, const glm::vec2& atlasSize);
		uint32_t GetId() const;
		/* 4 */
		const glm::vec2* GetTexCoords() const { return m_TexCoords; };
		const Ref<Texture>& GetTexture() const { return m_Texture; }
		Ref<Texture> GetTexture() { return m_Texture; }
		operator bool() const 
		{ 
			if(m_Texture) return m_Texture->GetId(); 
			return false;
		}
	};
}
}
