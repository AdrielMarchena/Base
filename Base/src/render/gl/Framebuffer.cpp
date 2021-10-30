#include "Framebuffer.h"

#include "gl/glew.h"
#include "utils/gl_error_macro_db.h"
#include "utils/base_assert.h"

namespace Base
{
	Framebuffer::Framebuffer(const FramebufferSpecification& specs)
		:m_Id(NULL),m_Specs(specs)
	{
		Invalidate();
	}

	Framebuffer::~Framebuffer()
	{
		if (m_Id)
		{
			GLCall(glDeleteFramebuffers(1, &m_Id));
			m_Id = NULL;
		}
		if (m_DepthBuffer)
		{
			glDeleteRenderbuffers(1, &m_DepthBuffer);
			m_DepthBuffer = NULL;
		}
	}

	void Framebuffer::Invalidate()
	{
		GLCall(glGenFramebuffers(1, &m_Id));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER,m_Id));
		BASE_CORE_ASSERT(m_Id, "Framebuffer could not be created");
		
		//Color Texture
		render::ImageInformation info;
		info.Buffer = nullptr;
		info.Width = m_Specs.width;
		info.Height = m_Specs.height;
		info.Channels = 4;
		info.MinFilter = render::GL_TextureFilter::LINEAR;
		info.MagFilter = render::GL_TextureFilter::LINEAR;
		info.WrapS = render::GL_TextureWrap::CLAMP_EDGE;
		info.WrapT = render::GL_TextureWrap::CLAMP_EDGE;
		info.GenerateMipMap = false;
		m_ColorTexture = render::Texture::CreateTexture(info);

		GLCall(glGenRenderbuffers(1, &m_DepthBuffer));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Specs.width, m_Specs.height));
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);

		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture->GetId(), 0));
		
		//Depth Texture
		info.InternalFormat = GL_DEPTH24_STENCIL8;
		info.DataFormat = GL_DEPTH_STENCIL;
		info.Type = GL_UNSIGNED_INT_24_8;
		m_DepthTexture = render::Texture::CreateTexture(info);
		
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture->GetId(), 0));
		
		GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, DrawBuffers);

		GLCall(BASE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,"Framebuffer incomplete"));

		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void Framebuffer::Bind()
	{
		BASE_ASSERT(m_Id);
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Id));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer));
	}

	void Framebuffer::Unbind()
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	}

	Ref<Framebuffer> Framebuffer::CreateFramebuffer(const FramebufferSpecification& specs)
	{
		return MakeRef<Framebuffer>(specs);
	}

}