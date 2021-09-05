/*
*	@file SubTexture.h
*	
*	Subtexture Stuff is here
*/

#pragma once
#include "Texture.h"
#include "glm/glm.hpp"
#include "gl/glew.h"
namespace en
{
namespace render
{

	class SubTexture
	{
	private:
		std::shared_ptr<Texture> m_Texture;
		glm::vec2 m_TexCoords[4] = { glm::vec2(0.0f) };
	public:
		SubTexture() = default;
		SubTexture(std::shared_ptr<Texture> texture , const glm::vec2& pos, const glm::vec2& size);
		static SubTexture CreateFromCoords(std::shared_ptr<Texture> texture, const glm::vec2 size, const glm::vec2& coords, const glm::vec2& spriteSize);
		GLuint GetId() const;
		/* 4 */
		const glm::vec2* GetTexCoords() const { return m_TexCoords; };

		operator bool() const 
		{ 
			if(m_Texture) return m_Texture->GetId(); 
			return false;
		}
	};
}
}
