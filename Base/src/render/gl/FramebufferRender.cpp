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
		info.MinFilter = render::GL_TextureFilter::LINEAR;
		info.MagFilter = render::GL_TextureFilter::LINEAR;
		info.WrapS = render::GL_TextureWrap::REPEAT;
		info.WrapT = render::GL_TextureWrap::REPEAT;
		//info.WrapR = render::GL_TextureWrap::CLAMP_EDGE;
		m_CurrentLookUpTable = MakeRef<render::Texture>(info);

		m_LookUpTables[m_CurrentLookUpTable->GetName()] = m_CurrentLookUpTable;

		//m_CurrentLookUpTable = render::Texture::GetWhiteTexture();

		//Create ShaderLib
		m_Shaders = MakeScope<render::ShaderLib>();
		m_Shaders->Add(render::Shader::CreateShader("shaders/Framebuffer.glsl"));
		m_CurrentShader = m_Shaders->Get("Framebuffer");
		m_CurrentShader->Bind();
		SetUpShader();

		//Create FrameBuffer
		InvalidadeFrameBuffer();

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
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Framebuffer->GetColorTexture()->GetId()));

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

	void FramebufferRender::SetUpShader()
	{
		float u_Width = m_CurrentLookUpTable->GetInformation().Width;
		float u_Height = m_CurrentLookUpTable->GetInformation().Height;
		float u_Colors = 16.0f;
		int u_UseGrade = m_Specs.use_grade ? 1 : 0;

		m_CurrentShader->SetUniform1i("u_Framebuffer", 0);
		m_CurrentShader->SetUniform1f("u_Width", u_Width);
		m_CurrentShader->SetUniform1f("u_Height", u_Height);
		m_CurrentShader->SetUniform1i("u_UseGrade", u_UseGrade);
		if (m_Specs.use_grade)
			m_CurrentShader->SetUniform1i("u_GradeLut", 1);
	}
}