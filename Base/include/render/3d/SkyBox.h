#pragma once

#include "glm/glm.hpp"
#include "Base/Base.h"
#include "render/gl/VertexBuffer.h"
#include "render/gl/VertexArray.h"
#include "render/gl/VertexAttribute.h"
#include "render/gl/IndexBuffer.h"
#include "render/gl/Shader.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace Base
{
	class SkyBox
	{
	private:
		uint32_t m_CubeTextureId;
		Ref<render::Shader> m_Shader;

		Ref<render::VertexBuffer> m_VB;
		Ref<render::VertexArray> m_VA;
		glm::mat4 m_ProjView;
	public:
		SkyBox(const std::vector<std::string>& paths, Ref<render::Shader> shader);

		~SkyBox();

		static Ref<SkyBox> CreateSkyBox(const std::vector<std::string>& paths, Ref<render::Shader> shader);
		static Ref<SkyBox> CreateSkyBox(const std::vector<std::string>& paths, const std::string& shader_path);
		
		void SetViewProj(const glm::mat4& proj, const glm::mat4& view);
		void Draw();

	private:
		static uint32_t LoadCubeMap(const std::vector<std::string>& paths);
	};
}


