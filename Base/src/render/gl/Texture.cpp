#include "Texture.h"

#include "utils/gl_error_macro_db.h"
#include "gl/glew.h"
#include "utils/base_assert.h"

#include "stb_image.h"
#include "lodepng.h"

#include "utils/Generic.h"

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
		case GL_TextureFilter::NEAREST_MIPMAP_NEAREST:
			return GL_NEAREST_MIPMAP_NEAREST;
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

	Texture::Texture(const ImageInformation& info, const std::string& path)
		:m_ImageInfo(info),m_Path(path),m_Name(info.Name)
	{
		BASE_CORE_ASSERT(info.Width > 0 &&
			info.Height > 0 &&
			info.Channels > 2 &&
			info.Channels < 5
		,"Some ImageInformation is wrong");

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
		if(info.GenerateMipMap)
			glGenerateMipmap(GL_TEXTURE_2D);

		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		if (!info.KeepSourceBuffer)
		{
			m_ImageInfo.Buffer = nullptr;
		}

		if (info.CopySourceBuffer)
		{
			size_t buffer_size = static_cast<size_t>(info.Width) * info.Height * info.Channels * sizeof(TextureBufferType);
			m_ImageInfo.Buffer = CreateTextureBuffer(buffer_size);
			memcpy_s(m_ImageInfo.Buffer, buffer_size, info.Buffer, buffer_size);
		}

		if (m_ImageInfo.DeleteSourceBuffer && !info.KeepSourceBuffer)
			DeleteTextureBuffer(info.Buffer);

		if (m_Name == "" || m_Name.empty())
		{
			if (m_Path == "" || m_Path.empty())
				m_Name = std::string(Base_Unamed_Texture);
			else
			{
				auto lastSlash = m_Path.find_last_of("/\\");
				lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
				auto lastDot = m_Path.rfind('.');
				auto count = lastDot == std::string::npos ? m_Path.size() - lastSlash : lastDot - lastSlash;
				m_Name = path.substr(lastSlash, count);
			}
		}
	}

	Texture::~Texture()
	{
		if (m_Id)
		{
			glDeleteTextures(1, &m_Id);
			m_Id = NULL;
		}
		if (m_ImageInfo.Buffer)
		{
			DeleteTextureBuffer(m_ImageInfo.Buffer);
			m_ImageInfo.Buffer = nullptr;
		}
	}

	Ref<Texture> Texture::CreateTexture(ImageInformation& info)
	{
		return MakeRef<Texture>(info);
	}

	Ref<Texture> Texture::CreateTexture(const std::string& path, const std::string& name)
	{
		auto info = GetImageInfo(path);
		info.Name = name;
		info.KeepSourceBuffer = true;
		return MakeRef<Texture>(GetImageInfo(path),path);
	}

	void Texture::CreatePng(const std::string& path, const ImageInformation& image_info)
	{
		BASE_ASSERT(image_info.Width > 0 && image_info.Height > 0 && image_info.Buffer != nullptr && path != "" && image_info.Channels > 2);
		auto type = image_info.Channels == 4 ? LodePNGColorType::LCT_RGBA : LodePNGColorType::LCT_RGB;
		lodepng::encode(path, image_info.Buffer, image_info.Width, image_info.Height, type);
	}

	void Texture::CreatePng(const std::string& path) const
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
			info.Buffer = CreateTextureBuffer(1,1,info.Channels);
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

	TextureBufferType* Texture::CreateTextureBuffer(size_t size)
	{
		BASE_CORE_ASSERT(size > 0,"Buffer size can't be 0");
		return (TextureBufferType*)malloc(size);
	}

	TextureBufferType* Texture::CreateTextureBuffer(unsigned int w, unsigned int h, unsigned int channels)
	{
		return CreateTextureBuffer(static_cast<size_t>(w) * h * channels * sizeof(TextureBufferType));
	}

	void Texture::DeleteTextureBuffer(TextureBufferType* buffer)
	{
		if(buffer)
			free(buffer);
	}

	void Texture::DeleteWhiteTexture()
	{
		m_WhiteTexture.reset();
	}

	ImageInformation Texture::GetImageInfo(const std::string& path)
	{
		const std::string ext = utils::ToLower(path.substr(path.find_last_of('.') + 1));
		
		if (ext == "png")
		{
			return GetImageInfoLodePNG(path);
		}
		
		return GetImageInfoStbi(path);
	}

	ImageInformation Texture::GetImageInfoLodePNG(const std::string& path)
	{
		ImageInformation info;

		//TODO: Flip vertically
		unsigned w = 0, h = 0;
		std::vector<unsigned char> image;

		unsigned error = lodepng::decode(image, w, h, path);
		if (error) //TODO: Deal properly with this error
			BASE_CORE_ASSERT(false,lodepng_error_text(error));

		info.Width = w;
		info.Height = h;
		info.Channels = 4;
		auto size = image.size();
		info.Buffer = CreateTextureBuffer(image.size());
		/*TextureBufferType* BufferPtr = info.Buffer;
		
		unsigned char* buff = image.data();
		unsigned char* last_ptr = &image.back();

		for (int i = h - 1; i >= 0; i--)
		{
			unsigned char* ptr = last_ptr - w;
			for (int j = 0; j < w; j++)
			{
				*BufferPtr = *ptr;
				BufferPtr++;
				ptr++;
			}
			if (last_ptr <= info.Buffer)
				break;
			last_ptr = ptr;
		}*/

		std::copy(image.begin(), image.end(), info.Buffer);
		image.clear();

		info.DeleteSourceBuffer = false;
		info.CopySourceBuffer = false;
		info.KeepSourceBuffer = true;
		return info;
	}

	ImageInformation Texture::GetImageInfoStbi(const std::string& path)
	{
		//TODO: Fix this stbi 'no SOI' bullshit 
		ImageInformation info;

		stbi_set_flip_vertically_on_load(1);
		auto px = stbi_load(path.c_str(), &info.Width, &info.Height, &info.Channels, 0);

		const char* er = stbi_failure_reason();
		if (er)
		{
			if(px)
				stbi_image_free(px);
			info.Buffer = nullptr;
			BASE_CORE_ASSERT(false, "Failed to load image");
		}
		info.Buffer = px;

		info.DeleteSourceBuffer = false;
		info.CopySourceBuffer = false;
		info.KeepSourceBuffer = true;
		return info;
	}

}
}