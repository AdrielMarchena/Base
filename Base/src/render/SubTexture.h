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
		const Texture& m_Texture;
		glm::vec2 m_TexCoords[4] = { glm::vec2(0.0f) };
	public:
		SubTexture();
		SubTexture(const Texture& texture, const glm::vec2& pos, const glm::vec2& size);
		static SubTexture CreateFromCoords(const Texture& texture, const glm::vec2 size, const glm::vec2& coords, const glm::vec2& spriteSize);
		GLuint GetId() const;
		/* 4 */
		const glm::vec2* GetTexCoords() const { return m_TexCoords; };
	};
}
}
