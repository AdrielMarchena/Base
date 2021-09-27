#pragma once

#include "Base/Base.h"
#include "glm/glm.hpp"
#include <string>
namespace Base
{
namespace render
{
	typedef uint8_t TextureBufferType;

	//TODO: Add enum to specify mipmap things (maybe)
	enum class GL_TextureFilter : uint8_t
	{
		LINEAR = 1,
		NEAREST,
		LINEAR_MIPMAP_LINEAR

	};
	enum class GL_TextureWrap : uint8_t
	{
		REPEAT = 1,
		MIRROR_REPEAT,
		CLAMP_EDGE,
		CLAMP_BORDER,
	};
	
	//unsigned int GL_SwithTextureFilter(GL_TextureFilter& filter_type);
	//unsigned int GL_SwithTextureWrap(GL_TextureWrap& wrap_type);

	struct ImageInformation
	{
		int32_t Width = 1;
		int32_t Height = 1;
		int32_t Channels = 4;
		TextureBufferType* Buffer = nullptr;
		
		uint32_t UnpackAligment = 1;
		GL_TextureFilter MinFilter = GL_TextureFilter::LINEAR_MIPMAP_LINEAR;
		GL_TextureFilter MagFilter = GL_TextureFilter::LINEAR;
		GL_TextureWrap WrapS = GL_TextureWrap::CLAMP_EDGE;
		GL_TextureWrap WrapT = GL_TextureWrap::CLAMP_EDGE;

		ImageInformation(const ImageInformation&) = default;
		ImageInformation() = default;

		bool DeleteSourceBuffer = false; // Set to true so the class delete the source buffer
	};

	class Texture
	{
	private:
		uint32_t m_Id = NULL;
		ImageInformation m_ImageInfo;
		static Ref<Texture> m_WhiteTexture;
	public:
		Texture(const ImageInformation& info);
		Texture() = default;

		~Texture();

		static Ref<Texture> CreateTexture(ImageInformation& info);
		static Ref<Texture> CreateTexture(const std::string& path);

		uint32_t GetId() const { return m_Id; };

		static void CreatePng(const std::string& path,const ImageInformation& image_info);
		void CreatePng(const std::string& path);

		const ImageInformation& GetInformation() const { return m_ImageInfo; }
		ImageInformation& GetInformation() { return m_ImageInfo; }

		static Ref<Texture> GetWhiteTexture();
		/* */

		static void DeleteWhiteTexture();

		glm::vec2 GetSize() const { return glm::vec2(m_ImageInfo.Width, m_ImageInfo.Height); }

		operator bool() const { return (bool)m_Id; }
	private:
		static ImageInformation GetImageInfo(const std::string& path);
		static ImageInformation GetImageInfoLodePNG(const std::string& path);
	};
}
}