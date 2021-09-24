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
#include "Base/Base.h"
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
		Texture(uint32_t wid, uint32_t hei, const void* data);
		Texture(uint32_t wid, uint32_t hei, unsigned char* data);
		~Texture();
		/* */

		uint32_t GetId() const { return m_Id; };
		glm::vec2 GetSize() const { return { m_Wid ,m_Hei }; }

		static ImageInfo GetImage(const char* path);
		static ImageInfo GetPNGImage(const char* path);
		static ResourceManager<Texture> LoadAsyncTexture(const std::vector<std::pair<std::string, std::string>>& names, const utils::NameCaps& nameCaps = utils::NameCaps::NONE, uint8_t batchLimit = 5);
		
		static Ref<Texture> CreateTexture(uint32_t wid, uint32_t hei, const void* data);
		static Ref<Texture> CreateTexture(uint32_t wid, uint32_t hei, unsigned char* data);

		static Ref<Texture> WhiteTexture()
		{

			static Ref<Texture> white_texture;
			static bool once = [&]() {
				//3D array
				unsigned char* texture_buffer = new unsigned char[1 * 1 * 4];

				for (unsigned int ix = 0; ix < 1; ++ix)
				{
					for (unsigned int iy = 0; iy < 1; ++iy)
					{
						texture_buffer[ix * 1 * 4 + iy * 4 + 0] = 0xff; //red
						texture_buffer[ix * 1 * 4 + iy * 4 + 1] = 0xff; //green
						texture_buffer[ix * 1 * 4 + iy * 4 + 2] = 0xff; //blue
						texture_buffer[ix * 1 * 4 + iy * 4 + 3] = 0xff; //alpha
					}
				}

				white_texture = CreateTexture(1, 1, (unsigned char*)texture_buffer);
				return true;
			}();

			return white_texture;
		};

		static void CreatePNGFile(const std::string& path, unsigned int w, unsigned int h, unsigned char* buffer);

		operator bool() const { return (bool)m_Id; }

		//TODO: remove, is used by Text thing
		void SetID(uint32_t new_id) { m_Id = new_id; }
		void Dispose();
	};
}
}


