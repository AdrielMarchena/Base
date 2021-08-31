/*
* @file Texture.h
*
* Texture Stuff
*/

#pragma once

#include <stdint.h>
#include <string>
#include <unordered_map>
#include "gl/glew.h"
#include "glm/glm.hpp"
#include "utils/NameCaps.h"
#include "utils/gl_error_macro_db.h"
#include "ResourceManager.h"
namespace en
{
namespace render
{
	struct ImageInfo
	{
		int32_t  m_Wid = NULL;
		int32_t  m_Hei = NULL;
		int32_t  m_Bit = NULL;
		uint8_t* m_Pixels = nullptr;
		bool png = false;
		void clear();
		//~ImageInfo() { clear(); }
	};

	class Texture
	{
	private:
		uint32_t m_Id  = NULL;
		int32_t  m_Wid = NULL;
		int32_t  m_Hei = NULL;
		int32_t  m_Bit = NULL;
		bool copy = false; //Maybe is trash code?
		bool disposed = false;
	public:
		Texture() = default;
		Texture(const std::string& path);
		Texture(const ImageInfo& info);
		~Texture() 
		{ 
			Dispose();
		};
		
		Texture(Texture& other) noexcept
		{
			m_Id = other.m_Id;
			m_Wid = other.m_Wid;
			m_Hei = other.m_Hei;
			m_Bit = other.m_Bit;
			this->copy = true;
		}
		Texture(Texture&& other) noexcept :copy(false)
		{
			m_Id =  other.m_Id;
			m_Wid = other.m_Wid;
			m_Hei = other.m_Hei;
			m_Bit = other.m_Bit;

			other.m_Id =  NULL;
			other.m_Wid = NULL;
			other.m_Hei = NULL;
			other.m_Bit = NULL;
			other.copy = false;
		}
		Texture& operator=(Texture& other) noexcept
		{
			if (this == &other)
				return *this;
			m_Id = other.m_Id;
			m_Wid = other.m_Wid;
			m_Hei = other.m_Hei;
			m_Bit = other.m_Bit;
			this->copy = true;
			return *this;
		}
		Texture& operator=(Texture&& other) noexcept
		{
			if (this == &other)
				return *this;

			if (deletable() && m_Id)
				Dispose();
			//Delete any heap alocated here

			m_Id = other.m_Id;
			m_Wid = other.m_Wid;
			m_Hei = other.m_Hei;
			m_Bit = other.m_Bit;

			other.m_Id =  NULL;
			other.m_Wid = NULL;
			other.m_Hei = NULL;
			other.m_Bit = NULL;
			other.copy = false;

			this->copy = false;
			return *this;
		}
		uint32_t GetId() const { return m_Id; };
		glm::vec2 GetSize() const { return { m_Wid ,m_Hei }; }

		static ImageInfo GetImage(const char* path);
		static ImageInfo GetPNGImage(const char* path);
		static ResourceManager<Texture> LoadAsyncTexture(const std::vector<std::pair<std::string, std::string>>& names, const utils::NameCaps& nameCaps = utils::NameCaps::NONE, uint8_t batchLimit = 5);
		static Texture& GetBlanckTexture()
		{
			static Texture tmp;
			return tmp;
		}

		bool IsCopy() const { return copy; }

		//Used only for draw Text
		void SetID(GLuint new_id) { m_Id = new_id; }
		void Dispose()
		{
			if(!disposed)
			{
				if (m_Id)
				{
					GLCall(glDeleteTextures(1, &m_Id));
					m_Id = NULL;
					disposed = true;
				}
			}
		}
	private:
		inline bool deletable() const
		{
			return (m_Id && !copy);
		}
	};
}
}


