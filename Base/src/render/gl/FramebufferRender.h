#pragma once

#include "Base/Base.h"
#include "base_gl.h"
#include "glm/glm.hpp"
#include "Framebuffer.h"
#include "Texture.h"
#include "render/Camera.h"
#include <unordered_map>
namespace Base
{
	struct FramebufferQuad
	{
		glm::vec3 Position;
		glm::vec2 TexCoords;
	};

	struct FrameBufferRenderSpecification
	{
		uint32_t width = 800;
		uint32_t height = 600;
		float scale_factor = 1.0f; //Is multiplied by width and height to scale them 
		bool use_grade = true; //if true, use the framebuffer size to create lookup tables
	};

	struct FramebufferPostEffect
	{
		bool active = false;
		std::vector<glm::vec2> offsets;
		std::vector<float> kernel;
	};

	class FramebufferRender
	{
	private:
		FrameBufferRenderSpecification m_Specs;
		Ref<render::VertexArray> m_VA;
		Ref<render::VertexBuffer> m_VB;
		Ref<render::IndexBuffer> m_IB;
		std::unordered_map<std::string, Ref<render::Texture>> m_LookUpTables;
		Ref<render::Texture> m_CurrentLookUpTable;
		Scope<render::ShaderLib> m_Shaders;
		Ref<render::Shader> m_CurrentShader;
		Ref<Framebuffer> m_Framebuffer;

		FramebufferQuad m_Quad;

		FramebufferQuad* m_Buffer = nullptr;
		FramebufferQuad* m_BufferPtr = nullptr;

		std::unordered_map<std::string, FramebufferPostEffect> m_PostEffects;
		FramebufferPostEffect* m_CurrentPostEffect = nullptr;
	public:
		FramebufferRender(const FrameBufferRenderSpecification& spec);
		~FramebufferRender();

		void DrawFrameBuffer(const glm::mat4& quad_position, const Camera& camera, const glm::mat4& camera_transform);

		void InvalidadeFrameBuffer();

		void BindFrameBuffer();
		void UnbindFrameBuffer();

		const FrameBufferRenderSpecification& GetSpec() const { return m_Specs; }
		FrameBufferRenderSpecification& GetSpec() { return m_Specs; }

		/* If name is empty, will use shader.name */
		void AddShader(const Ref<render::Shader>& shader, const std::string& name = std::string());
		void UseShader(const std::string& name);

		/* If name is empty, will use texture.name */
		void AddLookUpTable(const Ref<render::Texture>& texture, const std::string& name = std::string());
		void UseLookUpTable(const std::string& name);

		void UsePostEffect(const std::string& name);
		const std::unordered_map<std::string, FramebufferPostEffect>& GetPostEffects() const { return m_PostEffects; }

		const std::unordered_map<std::string, Ref<render::Texture>>& GetLookUpTables() const { return m_LookUpTables; }
		Ref <render::Texture> GetCurrentLookTable() const { return m_CurrentLookUpTable; }

		Ref <render::Shader> GetCurrentShader() const { return m_CurrentShader; }
	private:
		void SetUpShader();
		void SetUpPostEffects();
		void UpdatePostEffects();
	};

}