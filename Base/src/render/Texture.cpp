/*
*	@file Texture.cpp
*
*	Texture Stuff
*
*	Implementation File
*/

#include "Texture.h"
#include "stb_image/stb_image.h"
#include "utils/threading.h"
#include <future>
#include <mutex>
#include "utils/Logs.h"
namespace en
{
namespace render
{
	Texture::Texture(const std::string& path)
	{
		ImageInfo info = GetImage(path.c_str());
		m_Wid = info.m_Wid;
		m_Hei = info.m_Hei;
		//TODO: handle empty m_Pixels
		glGenTextures(1, &m_Id);
		assert(m_Id >= 0);
		glBindTexture(GL_TEXTURE_2D, m_Id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Wid, m_Hei, 0, GL_RGBA, GL_UNSIGNED_BYTE, info.m_Pixels);
		glBindTexture(GL_TEXTURE_2D, 0);
		if (info.m_Pixels)
			stbi_image_free(info.m_Pixels);
	}

	Texture::Texture(const ImageInfo& info)
	{
		m_Wid = info.m_Wid;
		m_Hei = info.m_Hei;
		//TODO: handle empty m_Pixels
		glGenTextures(1, &m_Id);
		assert(m_Id >= 0);
		glBindTexture(GL_TEXTURE_2D, m_Id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Wid, m_Hei, 0, GL_RGBA, GL_UNSIGNED_BYTE, info.m_Pixels);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	ImageInfo Texture::GetImage(const char* path)
	{
		ImageInfo info;
		stbi_set_flip_vertically_on_load(1);
		
		info.m_Pixels = stbi_load(path, &info.m_Wid, &info.m_Hei, &info.m_Bit, 4);
		//TODO: Add er to exception message
		auto er = stbi_failure_reason();
		if (!info.m_Pixels || !er)
		{
			const std::string msg = "Could not load texture '" + std::string(path);
			throw std::exception(msg.c_str());
		}
		return info;
	}
	
	static inline void CreateTexture(std::unordered_map<std::string, Texture>& map, utils::ResourceLoads<std::string, ImageInfo>& info)
	{
		while (!info.isAllLoad())
		{
			for (auto& inf : info.resources)
			{
				if (info.futures[inf.first]._Is_ready())
				{
					map[inf.first] = Texture(inf.second);
					D_LOG("TEXTURE CREATED image: '" << inf.first << "' Loaded!");
					inf.second.clear();
					info.futures.erase(inf.first);
					info.resources.erase(inf.first);
					break;
				}
			}
		}
	}

	std::unordered_map<std::string, Texture> Texture::LoadAsyncTextures(const std::vector<std::pair<std::string, std::string>>& names,uint8_t batchLimit)
	{
		utils::ResourceLoads<std::string, ImageInfo> loads;
		auto lamb = [&](const std::string& name, const std::string& path) {
			try 
			{
				auto info = Texture::GetImage(path.c_str());
				if (info.m_Pixels != NULL)
				{
					D_LOG("image: '" << name << "' Loaded!");
					std::lock_guard<std::mutex> lock(loads.mutex);
					loads.resources[name] = info;
				}
			}
			catch (const std::exception& e)
			{
				std::lock_guard<std::mutex> lock(loads.mutex);
				std::cout << e.what() << std::endl;
			}
		};

		uint8_t count = 0;
		std::unordered_map<std::string, Texture> mm;
		for (auto& name : names)
		{
			loads.futures[name.first] = std::async(std::launch::async, lamb, name.first, name.second);
			count++;
			if (count >= batchLimit)
			{
				CreateTexture(mm, loads);
				loads.resources.clear();
				loads.futures.clear();
				count = 0;
			}
		}
		CreateTexture(mm, loads);
		return mm; //Move because texture is there
	}
	void ImageInfo::clear()
	{
		if (m_Pixels) delete[] m_Pixels;
	}
}
}