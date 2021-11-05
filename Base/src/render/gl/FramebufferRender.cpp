#include "FramebufferRender.h"

#include "gl/glew.h"
#include "utils/gl_error_macro_db.h"

namespace Base
{

	static glm::vec2 tex_coords[] = {
		{0.0f,0.0f},
		{1.0f,0.0f},
		{1.0f,1.0f},
		{0.0f,1.0f}
	};
	static glm::vec4 QuadVertexPositions[4];

	FramebufferRender::FramebufferRender(const FrameBufferRenderSpecification& spec)
		:m_Specs(spec)
	{
		//Create LookUpTable lut16
		auto info = render::Texture::GetImageInfo("resources/images/lookup.png");
		//info.Target = render::GL_TextureTarget::TEXTURE_3D;
		info.MinFilter = GL_TextureFilter::LINEAR;
		info.MagFilter = GL_TextureFilter::LINEAR;
		info.WrapS = GL_TextureWrap::REPEAT;
		info.WrapT = GL_TextureWrap::REPEAT;
		//info.WrapR = render::GL_TextureWrap::CLAMP_EDGE;
		m_CurrentLookUpTable = MakeRef<render::Texture>(info);

		m_LookUpTables[m_CurrentLookUpTable->GetName()] = m_CurrentLookUpTable;

		//Create ShaderLib
		m_Shaders = MakeScope<render::ShaderLib>();
		m_Shaders->Add(render::Shader::CreateShader("shaders/Framebuffer.glsl"));
		m_CurrentShader = m_Shaders->Get("Framebuffer");
		m_CurrentShader->Bind();

		//Create FrameBuffer
		InvalidadeFrameBuffer();
		UsePostEffect("sharpen");

		m_Buffer = new FramebufferQuad[4];
		m_BufferPtr = m_Buffer;

		m_VA = render::VertexArray::CreateVertexArray();

		m_VB = render::VertexBuffer::CreateVertexBuffer(sizeof(FramebufferQuad) * 4);

		render::VertexAttribute layout(m_VB);

		layout.AddLayoutFloat(3, sizeof(FramebufferQuad), (const void*)offsetof(FramebufferQuad, Position));
		layout.AddLayoutFloat(2, sizeof(FramebufferQuad), (const void*)offsetof(FramebufferQuad, TexCoords));

		uint32_t indices[6]{};
		uint32_t offset = 0;
		for (int i = 0; i < 6; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}
		m_IB = render::IndexBuffer::CreateIndexBuffer(sizeof(indices), indices);

		QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void FramebufferRender::InvalidadeFrameBuffer()
	{
		//Create FrameBuffer
		FramebufferSpecification frame_spec;
		frame_spec.Attachments = { FrambufferTextureFormat::RGBA8, FrambufferTextureFormat::DEPTH };
		frame_spec.width = m_Specs.width * m_Specs.scale_factor;
		frame_spec.height = m_Specs.height * m_Specs.scale_factor;
		m_Framebuffer = MakeScope<Framebuffer>(frame_spec);
		SetUpShader();
		SetUpPostEffects();
	}

	void FramebufferRender::BindFrameBuffer()
	{
		m_Framebuffer->Bind();
	}

	void FramebufferRender::UnbindFrameBuffer()
	{
		m_Framebuffer->Unbind();
	}

	FramebufferRender::~FramebufferRender()
	{
		delete m_Shaders.release();
		if(m_Buffer)
			delete m_Buffer;
	}

	void FramebufferRender::DrawFrameBuffer(const glm::mat4& quad_position, const Camera& camera, const glm::mat4& camera_transform)
	{
		GLCall(glDisable(GL_DEPTH_TEST));
		using namespace render;
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(camera_transform);
		m_CurrentShader->Bind();
		m_CurrentShader->SetUniformMat4f("u_ViewProj", viewProj);

		m_BufferPtr = m_Buffer;
		for (size_t i = 0; i < 4; i++)
		{
			m_BufferPtr->Position = quad_position * QuadVertexPositions[i];
			m_BufferPtr->TexCoords = tex_coords[i];
			m_BufferPtr++;
		}
		GLsizeiptr size = (uint8_t*)m_BufferPtr - (uint8_t*)m_Buffer;

		m_VB->SubData(size, m_Buffer);

		//render::GL_TextureTarget target = m_Framebuffer->GetColorTexture()->GetInformation().Target;

		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Framebuffer->GetColorTexture(0)));

		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_CurrentLookUpTable->GetId()));

		m_VA->Bind();
		GLCommands::GL_DrawElementsCall(GL_Target::TRIANGLES, 6, GL_Type::UNSIGNED_INT);

		GLCall(glEnable(GL_DEPTH_TEST));
	}

	void FramebufferRender::AddShader(const Ref<render::Shader>& shader, const std::string& name)
	{
		const std::string sn = name.empty() ? shader->GetName() : name;
		m_Shaders->Add(sn,shader);
	}
	
	void FramebufferRender::UseShader(const std::string& name)
	{
		m_CurrentShader = m_Shaders->Get(name);
		SetUpShader();
	}

	void FramebufferRender::AddLookUpTable(const Ref<render::Texture>& texture, const std::string& name)
	{
		const std::string tx = name.empty() ? texture->GetName() : name;
		m_LookUpTables[tx] = texture;
	}
	
	void FramebufferRender::UseLookUpTable(const std::string& name)
	{
		m_CurrentLookUpTable = m_LookUpTables[name];
	}
	/*Empty string will set to none*/
	void FramebufferRender::UsePostEffect(const std::string& name)
	{
		if (m_CurrentPostEffect)
			m_CurrentPostEffect->active = false;
		if (name.empty())
		{
			m_CurrentPostEffect = nullptr;
			return;
		}
		m_CurrentPostEffect = &m_PostEffects[name];
		m_CurrentPostEffect->active = true;
		UpdatePostEffects();
	}

	void FramebufferRender::SetUpPostEffects()
	{
		//I was getting a small value for the width and height here, 
		//it was a bug that make a nice effect actually (only with the sharpen kernel tho)
		float offset_x = 1.0f / m_Framebuffer->GetSpec().width;
		float offset_y = 1.0f / m_Framebuffer->GetSpec().height;

		std::vector<glm::vec2> offsets = 
		{
			glm::vec2(-offset_x, offset_y),  glm::vec2(0.0f, offset_y),  glm::vec2(offset_x,offset_y),
			glm::vec2(-offset_x, 0.0f),      glm::vec2(0.0f, 0.0f),      glm::vec2(offset_x,0.0f),
			glm::vec2(-offset_x, -offset_y), glm::vec2(0.0f, -offset_y), glm::vec2(offset_x,-offset_y)
		};

		FramebufferPostEffect identity;
		identity.offsets = offsets;
		identity.kernel =
		{
			 0.0f, 0.0f, 0.0f,
			 0.0f, 1.0f, 0.0f,
			 0.0f, 0.0f, 0.0f,
		};
		m_PostEffects["identity"] = identity;

		FramebufferPostEffect sharpen;
		sharpen.offsets = offsets;
		sharpen.kernel = 
		{
			 0.0f,-1.0f, 0.0f,
			-1.0f, 5.0f,-1.0f,
			 0.0f,-1.0f, 0.0f
		};
		m_PostEffects["sharpen"] = sharpen;

		FramebufferPostEffect box_blur;
		box_blur.offsets = offsets;
		float dec = 1.0f / 9.0f;
		box_blur.kernel =
		{
			dec,dec,dec,
			dec,dec,dec,
			dec,dec,dec
		};
		m_PostEffects["box_blur"] = box_blur;

		FramebufferPostEffect edge_detection0;
		edge_detection0.offsets = offsets;
		edge_detection0.kernel =
		{
			1.0f, 1.0f,1.0f,
			1.0f,-8.0f,1.0f,
			1.0f, 1.0f,1.0f
		};
		m_PostEffects["edge_detection0"] = edge_detection0;

		FramebufferPostEffect edge_detection1;
		edge_detection1.offsets = offsets;
		edge_detection1.kernel =
		{
		   -1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f,
		    1.0f, 0.0f,-1.0f
		};
		m_PostEffects["edge_detection1"] = edge_detection1;

		FramebufferPostEffect edge_detection2;
		edge_detection2.offsets = offsets;
		edge_detection2.kernel =
		{
			0.0f,-1.0f, 0.0f,
		   -1.0f, 4.0f,-1.0f,
			0.0f,-1.0f, 0.0f
		};
		m_PostEffects["edge_detection2"] = edge_detection2;

		FramebufferPostEffect edge_detection3;
		edge_detection3.offsets = offsets;
		edge_detection3.kernel =
		{
			-1.0f,-1.0f,-1.0f,
			-1.0f, 8.0f,-1.0f,
			-1.0f,-1.0f,-1.0f
		};
		m_PostEffects["edge_detection3"] = edge_detection3;

		FramebufferPostEffect gaussian_blur3x3;
		gaussian_blur3x3.offsets = offsets;
		dec = 1.0f/16.0f;
		gaussian_blur3x3.kernel =
		{
			1.0f * dec, 2.0f * dec, 1.0f * dec,
			2.0f * dec, 4.0f * dec, 2.0f * dec,
			1.0f * dec, 2.0f * dec, 1.0f * dec
		};
		m_PostEffects["gaussian_blur3x3"] = gaussian_blur3x3;

		FramebufferPostEffect gaussian_blur5x5;
		gaussian_blur5x5.offsets = offsets;
		dec = 1.0f / 256.0f;
		gaussian_blur5x5.kernel =
		{
			1.0f * dec, 4.0f * dec, 6.0f * dec, 4.0f * dec, 1.0f * dec,
			4.0f * dec,16.0f * dec,24.0f * dec,16.0f * dec, 4.0f * dec,
			6.0f * dec,24.0f * dec,36.0f * dec,24.0f * dec, 6.0f * dec,
			4.0f * dec,16.0f * dec,24.0f * dec,16.0f * dec, 4.0f * dec,
			1.0f * dec, 4.0f * dec, 6.0f * dec, 4.0f * dec, 1.0f * dec
		};
		m_PostEffects["gaussian_blur5x5"] = gaussian_blur5x5;

		FramebufferPostEffect unsharp_masking;
		unsharp_masking.offsets = offsets;
		dec = -(1.0f / 256.0f);
		unsharp_masking.kernel =
		{
			1.0f * dec, 4.0f * dec,  6.0f * dec, 4.0f * dec, 1.0f * dec,
			4.0f * dec,16.0f * dec, 24.0f * dec,16.0f * dec, 4.0f * dec,
			6.0f * dec,24.0f * dec,-476.0f *dec,24.0f * dec,6.0f * dec,
			4.0f * dec,16.0f * dec, 24.0f * dec,16.0f * dec, 4.0f * dec,
			1.0f * dec, 4.0f * dec,  6.0f * dec, 4.0f * dec, 1.0f * dec
		};
		m_PostEffects["unsharp_masking"] = unsharp_masking;
	}

	void FramebufferRender::UpdatePostEffects()
	{
		float offset_x = 1.0f / m_Framebuffer->GetSpec().width;
		float offset_y = 1.0f / m_Framebuffer->GetSpec().height;

		m_CurrentPostEffect->offsets = {
			glm::vec2(-offset_x, offset_y),  glm::vec2(0.0f, offset_y),  glm::vec2(offset_x,offset_y),
			glm::vec2(-offset_x, 0.0f),      glm::vec2(0.0f, 0.0f),      glm::vec2(offset_x,0.0f),
			glm::vec2(-offset_x, -offset_y), glm::vec2(0.0f, -offset_y), glm::vec2(offset_x,-offset_y)
		};
		SetUpShader();
	}

	void FramebufferRender::SetUpShader()
	{
		float u_Width = m_Framebuffer->GetSpec().width;
		float u_Height = m_Framebuffer->GetSpec().height;
		float u_Colors = 16.0f;
		int u_UseGrade = m_Specs.use_grade ? 1 : 0;

		m_CurrentShader->SetUniform1i("u_Framebuffer", 0);
		m_CurrentShader->SetUniform1f("u_Width", u_Width);
		m_CurrentShader->SetUniform1f("u_Height", u_Height);
		m_CurrentShader->SetUniform1i("u_UseGrade", u_UseGrade);
		if (m_Specs.use_grade)
			m_CurrentShader->SetUniform1i("u_GradeLut", 1);

		if (m_CurrentPostEffect)
		{
			int kernel_size = m_CurrentPostEffect->kernel.size();
			m_CurrentShader->SetUniform1i("kernel_size", kernel_size);
			m_CurrentShader->SetUniform1fv("kernel_5x5_slot", kernel_size, m_CurrentPostEffect->kernel.data());

			int offset_size = m_CurrentPostEffect->offsets.size();
			m_CurrentShader->SetUniform1i("offset_size", offset_size);
			m_CurrentShader->SetUniform1fv("offsets", offset_size, &m_CurrentPostEffect->offsets[0][0]);
		}
	}
}