#include "Texture.h"

#include "utils/gl_error_macro_db.h"
#include "gl/glew.h"
#include "utils/base_assert.h"

#include "stb_image/stb_image.h"
#include "lodepng.h"

namespace Base
{
namespace render 
{
	static inline unsigned int GL_SwithTextureFilter(GL_TextureFilter& filter_type)
	{
		switch (filter_type)
		{
		case GL_TextureFilter::LINEAR:
			return GL_LINEAR;
		case GL_TextureFilter::NEAREST:
			return GL_NEAREST;
		case GL_TextureFilter::LINEAR_MIPMAP_LINEAR:
			return GL_LINEAR_MIPMAP_LINEAR;
		default:
			BASE_CORE_ASSERT(false, "Unknow Texture Filter");
			break;
		}
		return 0;
	}
	static inline unsigned int GL_SwithTextureWrap(GL_TextureWrap& wrap_type)
	{
		switch (wrap_type)
		{
		case GL_TextureWrap::REPEAT:
			return GL_REPEAT;
		case GL_TextureWrap::MIRROR_REPEAT:
			return GL_MIRRORED_REPEAT;
		case GL_TextureWrap::CLAMP_EDGE:
			return GL_CLAMP_TO_EDGE;
		case GL_TextureWrap::CLAMP_BORDER:
			return GL_CLAMP_TO_BORDER;
		default:
			BASE_CORE_ASSERT(false, "Unknow Texture Wrap");
			break;
		}
		return 0;
	}

	Ref<Texture> Texture::m_WhiteTexture;

	Texture::Texture(const ImageInformation& info)
		:m_ImageInfo(info)
	{
		BASE_CORE_ASSERT(info.Width > 0 &&
			info.Height > 0 &&
			info.Buffer != nullptr &&
			info.Channels > 2 &&
			info.Channels < 5
		,"Some ImageInformation is wrong");

		if (m_ImageInfo.DeleteSourceBuffer)
		{
			size_t buffer_size = static_cast<size_t>(info.Width) * info.Height * info.Channels * sizeof(TextureBufferType);
			m_ImageInfo.Buffer = new TextureBufferType[buffer_size];
			memcpy_s(m_ImageInfo.Buffer, buffer_size, info.Buffer, buffer_size);
			delete[] info.Buffer;
		}

		GLenum internalFormat = 0, dataFormat = 0;
		if (info.Channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (info.Channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		BASE_CORE_ASSERT(internalFormat & dataFormat, "Format not supported");

		GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, info.UnpackAligment));
		GLCall(glGenTextures(1, &m_Id));
		BASE_CORE_ASSERT(m_Id, "Texture Id is 0");

		GLCall(glBindTexture(GL_TEXTURE_2D, m_Id));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_SwithTextureFilter(m_ImageInfo.MinFilter)));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_SwithTextureFilter(m_ImageInfo.MagFilter)));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_SwithTextureWrap(m_ImageInfo.WrapS)));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_SwithTextureWrap(m_ImageInfo.WrapT)));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 
							0, 
							internalFormat, 
							m_ImageInfo.Width, 
							m_ImageInfo.Height, 
							0, 
							dataFormat, 
							GL_UNSIGNED_BYTE, 
							m_ImageInfo.Buffer
				));
		glGenerateMipmap(GL_TEXTURE_2D);

		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	Texture::~Texture()
	{
		if (m_Id)
		{
			glDeleteTextures(1, &m_Id);
			m_Id = NULL;
		}
		if (m_ImageInfo.Buffer) 
			delete[] m_ImageInfo.Buffer;
	}

	Ref<Texture> Texture::CreateTexture(ImageInformation& info)
	{
		//Ref<Texture> tex = MakeRef<Texture>(info);
		return MakeRef<Texture>(info);
	}

	Ref<Texture> Texture::CreateTexture(const std::string& path)
	{
		//ImageInformation info = GetImageInfo(path);
		//Ref<Texture> tex = MakeRef<Texture>(info);
		return MakeRef<Texture>(GetImageInfo(path));
	}

	void Texture::CreatePng(const std::string& path, const ImageInformation& image_info)
	{
		BASE_ASSERT(image_info.Width > 0 && image_info.Height > 0 && image_info.Buffer != nullptr && path != "" && image_info.Channels > 2);
		auto type = image_info.Channels == 4 ? LodePNGColorType::LCT_RGBA : LodePNGColorType::LCT_RGB;
		lodepng::encode(path, image_info.Buffer, image_info.Width, image_info.Height, type);
	}

	void Texture::CreatePng(const std::string& path)
	{
		BASE_ASSERT(m_ImageInfo.Width > 0 && m_ImageInfo.Height > 0 && m_ImageInfo.Buffer != nullptr && path != "" && m_ImageInfo.Channels > 2);
		auto type = m_ImageInfo.Channels == 4 ? LodePNGColorType::LCT_RGBA : LodePNGColorType::LCT_RGB;
		lodepng::encode(path, m_ImageInfo.Buffer, m_ImageInfo.Width, m_ImageInfo.Height, type);
	}

	Ref<Texture> Texture::GetWhiteTexture()
	{
		static bool once = [&]()
		{
			ImageInformation info;
			info.Width = 1;
			info.Height = 1;
			info.Channels = 4;
			info.Buffer = new TextureBufferType[1 * 1 * info.Channels];
			info.DeleteSourceBuffer = false;
			info.MinFilter = GL_TextureFilter::NEAREST;
			info.MagFilter = GL_TextureFilter::NEAREST;
			info.WrapS = GL_TextureWrap::REPEAT;
			info.WrapT = GL_TextureWrap::REPEAT;

			for (unsigned int ix = 0; ix < 1; ++ix)
			{
				for (unsigned int iy = 0; iy < 1; ++iy)
				{
					info.Buffer[ix * 1 * 4 + iy * 4 + 0] = 0xff; //red
					info.Buffer[ix * 1 * 4 + iy * 4 + 1] = 0xff; //green
					info.Buffer[ix * 1 * 4 + iy * 4 + 2] = 0xff; //blue
					info.Buffer[ix * 1 * 4 + iy * 4 + 3] = 0xff; //alpha
				}
			}

			m_WhiteTexture = Texture::CreateTexture(info);
			return true;
		}();
		return m_WhiteTexture;
	}

	void Texture::DeleteWhiteTexture()
	{
		m_WhiteTexture.reset();
	}

	ImageInformation Texture::GetImageInfo(const std::string& path)
	{
		//TODO: Fix this stbi 'no SOI' bullshit 
		ImageInformation info;

		stbi_set_flip_vertically_on_load(1);
		auto px = stbi_load(path.c_str(), &info.Width, &info.Height, &info.Channels, 0);

		const char* er = stbi_failure_reason();
		if (er)
		{
			stbi_image_free(px);
			info.Buffer = nullptr;
			if (er == "no SOI")
				return GetImageInfoLodePNG(path);
			BASE_CORE_ASSERT(px, "Failed to load image");
		}

		info.Buffer = px;

		info.DeleteSourceBuffer = false;
		return info;

	}

	ImageInformation Texture::GetImageInfoLodePNG(const std::string& path)
	{
		ImageInformation info;

		//TODO: Flip vertically
		unsigned w, h;
		std::vector<unsigned char> image;

		unsigned error = lodepng::decode(image, w, h, path);

		if (error)
			BASE_CORE_ASSERT(false,lodepng_error_text(error));

		info.Width = w;
		info.Height = h;
		info.Buffer = new uint8_t[image.size() * sizeof(TextureBufferType)];
		std::copy(image.begin(), image.end(), info.Buffer);
		image.clear();

		info.DeleteSourceBuffer = false;
		return info;

	}

}
}