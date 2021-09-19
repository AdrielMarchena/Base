/*
* @file Texture.h
*
* Texture Stuff
*/

#pragma once

#include <stdint.h>
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include "utils/NameCaps.h"
#include "ResourceManager.h"
#include "gl/GL_Types.h"
namespace Base
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
		bool disposed = false;
		void Create(ImageInfo& info);
	public:
		Texture() = default;
		Texture(const std::string& path);
		Texture(ImageInfo& info);
		Texture(uint32_t color,uint32_t w, uint32_t);
		~Texture();
		/* */

		uint32_t GetId() const { return m_Id; };
		glm::vec2 GetSize() const { return { m_Wid ,m_Hei }; }

		static ImageInfo GetImage(const char* path);
		static ImageInfo GetPNGImage(const char* path);
		static ResourceManager<Texture> LoadAsyncTexture(const std::vector<std::pair<std::string, std::string>>& names, const utils::NameCaps& nameCaps = utils::NameCaps::NONE, uint8_t batchLimit = 5);
		
		static Ref<Texture> WhiteTexture()
		{
			static Ref<Texture> white_texture = MakeRef<Texture>(0xffffffff, 1, 1);
			return white_texture;
		};

		operator bool() const { return (bool)m_Id; }

		void SetID(uint32_t new_id) { m_Id = new_id; }
		void Dispose();
	};
}
}


