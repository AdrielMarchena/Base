#pragma once

#include "Base/Base.h"
#include "Texture.h"
#include <vector>
namespace Base
{

	enum class FrambufferTextureFormat
	{
		NONE = 0,
		RGBA8,
		DEPTH24STENCIL8,
		DEPTH = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FrambufferTextureFormat TextureFormat;

		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FrambufferTextureFormat format)
			:TextureFormat(format){}

	};

	struct FramebufferAttachmentSpecification
	{
		std::vector<FramebufferTextureSpecification> Attachment;

		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			:Attachment(attachments){}

	};

	struct FramebufferSpecification
	{
		uint32_t width;
		uint32_t height;
		FramebufferAttachmentSpecification Attachments;
		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	private:
		uint32_t m_Id;
		uint32_t m_DepthBuffer;
		Ref<render::Texture> m_ColorTexture;
		Ref<render::Texture> m_DepthTexture;
		FramebufferSpecification m_Specs;
	public:
		Framebuffer(const FramebufferSpecification& specs);
		~Framebuffer();

		Ref<render::Texture> GetColorTexture() const { return m_ColorTexture; }
		Ref<render::Texture> GetDepthTexture() const { return m_DepthTexture; }
		void Invalidate();

		void Bind();
		void Unbind();

		glm::vec2 GetSize() const { return {(float)m_Specs.width,(float)m_Specs.height}; }

		const FramebufferSpecification& GetSpec() const { return m_Specs; }
		static Ref<Framebuffer> CreateFramebuffer(const FramebufferSpecification& specs);
	};
}