/*
*	@file Texture.cpp
*
*	Texture Stuff
*
*	Implementation File
*/

#include "Texture.h"
#include "stb_image/stb_image.h"
#include "lodepng.h"
#include "utils/threading.h"
#include "utils/base_exceptions.h"
#include <future>
#include <mutex>
#include "Log.h"
#include "utils/Generic.h"
namespace Base
{
namespace render
{
	Texture::Texture(const std::string& path)
	{
		ImageInfo info = GetImage(path.c_str());
		if (!info.m_Pixels)
			throw std::exception("The image data is empty");
		m_Wid = info.m_Wid;
		m_Hei = info.m_Hei;

		GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

		GLCall(glGenTextures(1, &m_Id));
		if (!m_Id)
			throw std::exception("The image OpenGL_ID is empty");

		GLCall(glBindTexture(GL_TEXTURE_2D, m_Id));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Wid, m_Hei, 0, GL_RGBA, GL_UNSIGNED_BYTE, info.m_Pixels));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		info.clear();
	}

	Texture::Texture(const ImageInfo& info)
	{
		if (!info.m_Pixels)
			throw std::exception("The image data is empty");
	
		m_Wid = info.m_Wid;
		m_Hei = info.m_Hei;

		GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

		GLCall(glGenTextures(1, &m_Id));
		if (!m_Id)
			throw std::exception("OpenGL_ID is empty");
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Id));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Wid, m_Hei, 0, GL_RGBA, GL_UNSIGNED_BYTE, info.m_Pixels));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	ImageInfo Texture::GetImage(const char* path)
	{
		//TODO: If the stbi_load can't load the PNG, pngloader will
		//      But i think this is "loading twice"
		ImageInfo info;

		stbi_set_flip_vertically_on_load(1);

		auto px = stbi_load(path, &info.m_Wid, &info.m_Hei, &info.m_Bit, STBI_rgb_alpha);
	
		if (!px)
			throw std::exception("The pixels tried to be loaded are empty");
		info.m_Pixels = px;

		const char* er = stbi_failure_reason();
		if (er)
		{
			if (er == "no SOI")
				return GetPNGImage(path);
			throw std::exception(er);
		}
		return info;
	}

	ImageInfo Texture::GetPNGImage(const char* path)
	{
		//TODO: Flip vertically
		ImageInfo info;
		info.png = true;
		unsigned w, h;
		std::vector<unsigned char> image;

		unsigned error = lodepng::decode(image, w, h, path);

		if(error)
			throw std::exception(lodepng_error_text(error));

		info.m_Wid = w;
		info.m_Hei = h;
		//If i could steal the buffer inside the vector this would be much easy
		info.m_Pixels = new uint8_t[image.size()];
		std::copy(image.begin(),image.end(),info.m_Pixels);
		image.clear();
		return info;
	}

	static inline void TryCreate(ResourceManager<Texture>& map, utils::ResourceLoads<std::string, ImageInfo>& info, const utils::NameCaps& nameCaps)
	{
		for (auto& inf : info.resources)
		{
			if (info.futures[inf.first]._Is_ready())
			{
				std::string name = inf.first;
				switch (nameCaps)
				{
				case utils::NameCaps::NONE: break;
				case utils::NameCaps::ALL_LOWER: name = utils::ToLower(name); break;
				case utils::NameCaps::ALL_UPPER: name = utils::ToUpper(name); break;
				default: break;
				}
				try
				{
					map.AddResource(name, inf.second);
					//map[name] = std::move(Texture(inf.second));
					BASE_DEBUG("TEXTURE CREATED image: {0}", inf.first);
					info.futures[inf.first].~future();
					info.futures.erase(inf.first);
					info.resources[inf.first].clear();
					info.resources.erase(inf.first);
					break;
				}
				catch (const std::exception& ex)
				{
					BASE_ERROR("Can't Create Texture: '{0}', error: {1}", inf.first, ex.what());
					info.resources[inf.first].clear();
					info.futures.erase(inf.first);
					info.resources.erase(inf.first);
					//std::cout << "Can't Create Texture '" << inf.first << "' , error: " << ex.what() << std::endl;
					break;
				}
			}
		}
	}

	static inline void CreateTexture(ResourceManager<Texture>& map, utils::ResourceLoads<std::string, ImageInfo>& info, const utils::NameCaps& nameCaps)
	{
		using namespace utils;
		while (!info.isAllLoad())
		{
			std::lock_guard<std::mutex> lock(info.mutex);
			TryCreate(map, info, nameCaps);
		}
		while (!info.resources.empty())
		{
			std::lock_guard<std::mutex> lock(info.mutex);
			TryCreate(map, info, nameCaps);
		}
		info.futures.clear();
		info.resources.clear();
	}

	ResourceManager<Texture> Texture::LoadAsyncTexture(const std::vector<std::pair<std::string, std::string>>& names, const utils::NameCaps& nameCaps, uint8_t batchLimit)
	{
		utils::ResourceLoads<std::string, ImageInfo> loads;
		auto lamb = [&](const std::string& name, const std::string& path) {
			try
			{
				ImageInfo info;
				info = Texture::GetImage(path.c_str());
				if (info.m_Pixels != NULL)
				{
					std::lock_guard<std::mutex> lock(loads.mutex);
					BASE_TRACE("Image: '{0}' Loaded!", name);
					//D_LOG("image: '" << name << "' Loaded!");
					loads.resources[name] = info;
				}
			}
			catch (const std::exception& ex)
			{
				std::lock_guard<std::mutex> lock(loads.mutex);
				BASE_ERROR("Can't Load Image '{0}', error: {1}", name, ex.what());
				//std::cout << "Can't Load Image '" << name << "' , error: " << ex.what() << std::endl;
			}
		};

		uint8_t count = 0;
		ResourceManager<Texture> mm;
		for (auto& name : names)
		{
			if (count > batchLimit)
			{
				CreateTexture(mm, loads, nameCaps);
				loads.resources.clear();
				loads.futures.clear();
				count = 0;
			}
			loads.futures[name.first] = std::async(std::launch::async, lamb, name.first, name.second);
			//lamb(name.first, name.second);
			count++;
		}

		loads.waitAll();
		CreateTexture(mm, loads, nameCaps);
		return mm;
	}

	void ImageInfo::clear()
	{
		if (png)
		{
			if (m_Pixels)
				delete[] m_Pixels;
		}
		else	
			if (m_Pixels)
				stbi_image_free(m_Pixels);
	}
}
}