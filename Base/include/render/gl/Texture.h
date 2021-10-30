#pragma once

#include "Base/Base.h"
#include "glm/glm.hpp"
#include <string>
namespace Base
{
namespace render
{
	constexpr std::string_view Base_Unamed_Texture = "Unamed_Texture";
	typedef uint8_t TextureBufferType;

	//TODO: Add enum to specify mipmap things (maybe)
	enum class GL_TextureFilter : uint8_t
	{
		LINEAR = 1,
		NEAREST,
		LINEAR_MIPMAP_LINEAR,
		NEAREST_MIPMAP_NEAREST

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
		//Image details
		int32_t Width = 1;
		int32_t Height = 1;
		int32_t Channels = 4;
		/* 
		* if empty, will try get the name from the file
		* Example = "D:\Path\To\Texture.png" the name would be "Texture"
		*/
		std::string Name = ""; //if empty, the name will be the file name without extension
		TextureBufferType* Buffer = nullptr; //Texture Buffer
		
		//Texture details
		uint32_t UnpackAligment = 1;
		GL_TextureFilter MinFilter = GL_TextureFilter::NEAREST;
		GL_TextureFilter MagFilter = GL_TextureFilter::NEAREST;
		GL_TextureWrap WrapS = GL_TextureWrap::CLAMP_EDGE;
		GL_TextureWrap WrapT = GL_TextureWrap::CLAMP_EDGE;
		bool GenerateMipMap = true;

		ImageInformation(const ImageInformation&) = default;
		ImageInformation() = default;

		// Buffer options
		bool DeleteSourceBuffer = false; // Set to true to delete the buffer on texture creation
		bool KeepSourceBuffer = true; // Set to true to Keep the pointer to the source buffer
		bool CopySourceBuffer = false; // Set to true to Copy the buffer (memcpy_s)

		uint32_t InternalFormat = 0; //If zero, will guess using Channels value
		uint32_t DataFormat = 0; //If zero, will guess using Channels value
		uint32_t Type = 0; //If zero, GL_UNSIGNED_BYTE
	};

	class Texture
	{
	private:
		uint32_t m_Id = NULL;
		ImageInformation m_ImageInfo;
		std::string m_Path;
		std::string m_Name;
		static Ref<Texture> m_WhiteTexture;
	public:
		Texture(const ImageInformation& info ,const std::string& path = std::string());
		Texture() = default;

		~Texture();

		static Ref<Texture> CreateTexture(ImageInformation& info);
		static Ref<Texture> CreateTexture(const std::string& path, const std::string& name = std::string());

		uint32_t GetId() const { return m_Id; };
		const std::string& GetPath() const { return m_Path; }

		const std::string& GetName() const { return m_Name; }

		static void CreatePng(const std::string& path,const ImageInformation& image_info);
		void CreatePng(const std::string& path) const;

		const ImageInformation& GetInformation() const { return m_ImageInfo; }
		ImageInformation& GetInformation() { return m_ImageInfo; }

		static Ref<Texture> GetWhiteTexture();
		/* */

		//new Base::render::TextureBufferType[info.Width * info.Height * info.Channels * sizeof(Base::render::TextureBufferType)];
		static TextureBufferType* CreateTextureBuffer(size_t size);
		static TextureBufferType* CreateTextureBuffer(unsigned int w,unsigned int h,unsigned int channels);
		static void DeleteTextureBuffer(TextureBufferType* buffer);

		static void DeleteWhiteTexture();

		glm::vec2 GetSize() const { return glm::vec2(m_ImageInfo.Width, m_ImageInfo.Height); }

		operator bool() const { return (bool)m_Id; }
		static ImageInformation GetImageInfo(const std::string& path);
		static ImageInformation GetImageInfoLodePNG(const std::string& path);
		static ImageInformation GetImageInfoStbi(const std::string& path);
	private:
	};
}
}