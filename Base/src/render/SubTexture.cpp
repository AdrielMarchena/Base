/*
*	@file SubTexture.cpp
*	
*	Subtexture Stuff is here
*
*	Implementation File
*/

#include "SubTexture.h"

#include <iostream>
namespace en
{
namespace render
{
	SubTexture::SubTexture(std::shared_ptr<Texture> texture, const glm::vec2& min, const glm::vec2& max)
		:m_Texture(texture)
	{
		if (m_Texture->GetId())
		{
			m_TexCoords[0] = { min.x,min.y };
			m_TexCoords[1] = { max.x,min.y };
			m_TexCoords[2] = { max.x,max.y };
			m_TexCoords[3] = { min.x,max.y };
		}
	}

	SubTexture SubTexture::CreateFromCoords(std::shared_ptr<Texture> texture, const glm::vec2 size, const glm::vec2& coords, const glm::vec2& spriteSize)
	{
		float sheetWidth = size.x, sheetHeight = size.y;

		glm::vec2 min = { (coords.x * spriteSize.x) / sheetWidth, (coords.y * spriteSize.y) / sheetHeight };
		glm::vec2 max = { ((coords.x + 1) * spriteSize.x) / sheetWidth, ((coords.y + 1) * spriteSize.y) / sheetHeight };

		return SubTexture(texture, min, max);
	}

	GLuint SubTexture::GetId() const
	{
		if (!m_Texture->GetId())
		{
			return NULL;
		}
		return m_Texture->GetId();
	}
}
}

