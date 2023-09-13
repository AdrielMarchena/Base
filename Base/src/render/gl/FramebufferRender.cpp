#include "pch.h"
#include "FramebufferRender.h"

#include "glad/glad.h"
#include "utils/gl_error_macro_db.h"

#include "glm/glm.hpp"
#include <glm/gtx/quaternion.hpp>
#include "utils/Generic.h"
namespace Base {

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
		//Create ShaderLib
		m_Shaders = MakeScope<render::ShaderLib>();
		m_Shaders->Add(render::Shader::CreateShader("shaders/Framebuffer.glsl"));
		m_CurrentShader = m_Shaders->Get("Framebuffer");
		m_CurrentShader->Bind();

		//Create FrameBuffer
		SetUpFramebuffer();
		SetUpShader();

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
		QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void FramebufferRender::InvalidateFrameBuffer()
	{
		//Create FrameBuffer
		m_Framebuffer->m_Specs.width = m_Specs.width * m_Specs.scale_factor;
		m_Framebuffer->m_Specs.height = m_Specs.height * m_Specs.scale_factor;
		m_Framebuffer->Invalidate();
		SetUpShader();
	}

	void FramebufferRender::ResizeFramebuffer(uint32_t w, uint32_t h)
	{
		BASE_CORE_ASSERT(w > 0 && h > 0, "framebuffer width or height can't be zero");
		m_Specs.width = w;
		m_Specs.height = h;
		InvalidateFrameBuffer();
	}

	void FramebufferRender::BindFrameBuffer()
	{
		m_Framebuffer->Bind();
		GLCall(glViewport(0, 0, m_Specs.width * m_Specs.scale_factor, m_Specs.height * m_Specs.scale_factor));
	}

	void FramebufferRender::UnbindFrameBuffer()
	{
		m_Framebuffer->Unbind();
		GLCall(glViewport(0, 0, m_Specs.width, m_Specs.height));
	}

	FramebufferRender::~FramebufferRender()
	{
		delete m_Shaders.release();
		if (m_Buffer)
			delete m_Buffer;
	}

	void FramebufferRender::DrawFrameBuffer(const Camera& camera, const glm::mat4& camera_transform)
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glDisable(GL_DEPTH_TEST));
		using namespace render;
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(camera_transform);
		m_CurrentShader->Bind();
		m_CurrentShader->SetUniformMat4f("u_ViewProj", viewProj);

		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Framebuffer->GetColorTexture(0)));

		m_VA->Bind();
		GLCommands::GL_DrawElementsCall(GL_Target::TRIANGLES, 6, GL_Type::UNSIGNED_INT);

		GLCall(glEnable(GL_DEPTH_TEST));
	}

	void FramebufferRender::AddShader(const Ref<render::Shader>& shader, const std::string& name)
	{
		const std::string sn = name.empty() ? shader->GetName() : name;
		m_Shaders->Add(sn, shader);
	}

	void FramebufferRender::UseShader(const std::string& name)
	{
		m_CurrentShader = m_Shaders->Get(name);
		SetUpShader();
	}

	void FramebufferRender::SetUpFramebuffer()
	{
		//Create FrameBuffer
		FramebufferSpecification frame_spec;
		frame_spec.Attachments = { FrambufferTextureFormat::RGBA8,FrambufferTextureFormat::RED_INTEGER,FrambufferTextureFormat::DEPTH };
		frame_spec.width = m_Specs.width * m_Specs.scale_factor;
		frame_spec.height = m_Specs.height * m_Specs.scale_factor;
		m_Framebuffer = MakeScope<Framebuffer>(frame_spec);
	}

	void FramebufferRender::SetUpShader()
	{
		float u_Width = m_Framebuffer->GetSpec().width;
		float u_Height = m_Framebuffer->GetSpec().height;

		m_CurrentShader->Bind();
		m_CurrentShader->SetUniform1i("u_Framebuffer", 0);
		m_CurrentShader->SetUniform1f("u_Width", u_Width);
		m_CurrentShader->SetUniform1f("u_Height", u_Height);
	}

	void FramebufferRender::CalculateQuadTransform()
	{
		glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));
		m_Transform = glm::translate(glm::mat4(1.0f), m_Position)
			* rotation
			* glm::scale(glm::mat4(1.0f), m_Scale);

		m_BufferPtr = m_Buffer;
		for (size_t i = 0; i < 4; i++)
		{
			m_BufferPtr->Position = m_Transform * QuadVertexPositions[i];
			m_BufferPtr->TexCoords = tex_coords[i];
			m_BufferPtr++;
		}
		GLsizeiptr size = (uint8_t*)m_BufferPtr - (uint8_t*)m_Buffer;
		m_VB->SubData(size, m_Buffer);
	}
}