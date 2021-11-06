#pragma once

#include "Base/Base.h"
#include "utils/base_assert.h"
#include "Texture.h"
#include <vector>
#include "FramebufferSpecifications.h"
namespace Base
{
	class FramebufferRender;
	class Framebuffer
	{
	private:
		uint32_t m_Id;
		uint32_t m_DepthBuffer;
		std::vector<uint32_t> m_ColorTextures;
		//uint32_t m_DepthTexture;
		FramebufferSpecification m_Specs;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;
		FramebufferTextureSpecification m_DepthAttachmentSpec;
	public:
		Framebuffer(const FramebufferSpecification& specs);
		~Framebuffer();

		uint32_t GetColorTexture(uint32_t index = 0) const { BASE_ASSERT(index < m_ColorTextures.size()); return m_ColorTextures[index]; }
		//Ref<render::Texture> GetDepthTexture() const { return m_DepthTexture; }
		void Invalidate();

		void Bind();
		void Unbind();

		glm::vec2 GetSize() const { return {(float)m_Specs.width,(float)m_Specs.height}; }

		const FramebufferSpecification& GetSpec() const { return m_Specs; }
		FramebufferSpecification& GetSpec() { return m_Specs; }
		static Ref<Framebuffer> CreateFramebuffer(const FramebufferSpecification& specs);

	private:
		friend FramebufferRender;
	};
}