#pragma once

#include "Base/Base.h"
#include "Model.h"
#include "render/gl/Shader.h"
#include "render/gl/Gl_Commands.h"
#include "scene/SceneCamera.h"
#include "SkyBox.h"
#include "scene/CameraScript.h"
namespace Base
{
	class Render3D
	{
		static size_t m_LastSize;
		static size_t m_BatchLimit;
		static size_t m_CurrentBatchSize;
		static Scope<render::ShaderLib> m_Shaders;
		//static Scope<std::unordered_map< Ref<std::vector<Mesh>>, glm::mat4>> m_Meshes;
		static Scope<std::vector<std::pair<Ref<std::vector<Mesh>>, glm::mat4>>> m_Meshes;
		static Scope<SkyBox> m_SkyBox;
	public:

		//Render3D();
		//~Render3D() { m_Meshes.clear(); }

		static void Clear(GL_ClearCommand command = GL_ClearCommand::ClearDepthColor);

		static void Init();
		static void Dispose();

		static void SetSkyBox(Ref<render::Shader> shader,const std::vector<std::string>& faces);
		static void SetSkyBox(Ref<render::Shader> shader,const std::string& dir);
		static void SetSkyBox(const std::string& shader,const std::vector<std::string>& faces);
		static void SetSkyBox(const std::string& shader,const std::string& dir);

		static void StartScene(const Camera& camera, const glm::mat4& camera_transform);
		static void StartBatch();
		static void Flush();
		static void EndBatch();
		static void EndScene();

		static void SubmitMesh(Ref<std::vector<Mesh>>& mesh, const glm::mat4& model_matrix);
	};
}