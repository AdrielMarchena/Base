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
		while (!info.m_Pixels)
		{
			static uint8_t count = 0;
			info.m_Pixels = stbi_load(path, &info.m_Wid, &info.m_Hei, &info.m_Bit, 4);
			if (!stbi_failure_reason())
				std::cout << stbi_failure_reason() << std::endl;
			if (count >= 10)
			{
				std::cout << "Could not load texture '" << path << "' Error: " << stbi_failure_reason() << std::endl;
				break;
			}
			count++;
		}
		return info;
	}
	
	std::unordered_map<std::string, Texture> Texture::LoadAsyncTextures(const std::vector<std::pair<std::string, std::string>>& names)
	{
		utils::ResourceLoads<std::string, ImageInfo> loads;
		auto lamb = [&](const std::string& name, const std::string& path) {
			using namespace render;
			auto info = Texture::GetImage(path.c_str());
			if (info.m_Pixels != NULL)
			{
				D_LOG("image: '" << name << "' Loaded!");
				std::lock_guard<std::mutex> lock(loads.mutex);
				loads.resources[name] = info;
			}
		};

		for (auto& name : names)
		{
			loads.futures[name.first] = std::async(std::launch::async, lamb, name.first, name.second);
		}

		std::unordered_map<std::string, Texture> mm;
		while (!loads.isAllLoad())
		{
			for (auto& inf : loads.resources)
			{
				if (loads.futures[inf.first]._Is_ready())
				{
					mm[inf.first] = Texture(inf.second);
					D_ACTION(if (mm[inf.first].GetId() != 0)
						D_LOG("TEXTURE CREATED image: '" << inf.first << "' Loaded!"));
					inf.second.clear();
					loads.futures.erase(inf.first);
					loads.resources.erase(inf.first);
					break;
				}
			}
		}
		return std::move(mm); //Move because texture is there
	}
	void ImageInfo::clear()
	{
		if (m_Pixels) delete[] m_Pixels;
	}
}
}