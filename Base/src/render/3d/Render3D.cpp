#include "Render3D.h"
#include "utils/Files.h"

namespace Base
{

	size_t Render3D::m_LastSize;
	size_t Render3D::m_BatchLimit;
	size_t Render3D::m_CurrentBatchSize;
	Scope<render::ShaderLib> Render3D::m_Shaders;
	Scope<std::vector<std::pair<Ref<std::vector<Mesh>>, glm::mat4>>> Render3D::m_Meshes;
	Scope<SkyBox> Render3D::m_SkyBox;
	void Render3D::Clear(GL_ClearCommand command)
	{
		render::GLCommands::GL_Clear(command);
	}

	void Render3D::Init()
	{
		m_LastSize = 1;
		m_BatchLimit = 500;
		m_CurrentBatchSize = 0;

		m_Shaders = MakeScope< render::ShaderLib>();
		m_Shaders->Add(render::Shader::CreateShader("shaders/3D.glsl"));

		m_Meshes = MakeScope<std::vector<std::pair<Ref<std::vector<Mesh>>, glm::mat4>>>();

	}

	void Render3D::Dispose()
	{
		delete m_Shaders.release();
		delete m_Meshes.release();
		if(m_SkyBox)
			delete m_SkyBox.release();
	}

	void Render3D::SetSkyBox(Ref<render::Shader> shader, const std::vector<std::string>& faces)
	{
		m_Shaders->Add(shader);
		m_SkyBox = MakeScope<SkyBox>(faces, shader);
	}

	void Render3D::SetSkyBox(Ref<render::Shader> shader, const std::string& dir)
	{
		auto files = Base::utils::Files::GetPairText(dir);
		std::vector<std::string> faces;
		faces.reserve(6);

		for (auto& pair : files)
			faces.push_back(pair.second);
		SetSkyBox(shader, faces);
	}

	void Render3D::SetSkyBox(const std::string& shader, const std::vector<std::string>& faces)
	{
		SetSkyBox(render::Shader::CreateShader(shader),faces);
	}

	void Render3D::SetSkyBox(const std::string& shader, const std::string& dir)
	{
		SetSkyBox(render::Shader::CreateShader(shader), dir);
	}

	void Render3D::StartScene(const Camera& camera, const glm::mat4& camera_transform)
	{
		auto shader = m_Shaders->Get("3D");
		shader->Bind();
		shader->SetUniformMat4f("projection", camera.GetProjection());
		shader->SetUniformMat4f("view", camera_transform);

		if(m_SkyBox)
			m_SkyBox->SetViewProj(camera.GetProjection(), camera_transform);
	}

	void Render3D::StartBatch()
	{
		m_Meshes->clear();
		m_Meshes->reserve(m_LastSize);
		m_CurrentBatchSize = 0;
	}

	void Render3D::Flush()
	{
		auto shader = m_Shaders->Get("3D");
		shader->Bind();
		for (auto& list : *m_Meshes)
		{
			shader->SetUniformMat4f("model", list.second);
			for(auto& mesh : *list.first)
				mesh.Draw(shader);
		}
	}

	void Render3D::EndBatch()
	{
		m_LastSize = m_Meshes->size();
	}

	void Render3D::EndScene()
	{
		if(m_SkyBox)
			m_SkyBox->Draw();
	}

	void Render3D::SubmitMesh(Ref<std::vector<Mesh>>& mesh, const glm::mat4& model_matrix)
	{
		if (m_CurrentBatchSize >= m_BatchLimit)
		{
			EndBatch();
			Flush();
			StartBatch();
		}
		m_Meshes->push_back(std::make_pair(mesh,model_matrix));
		//m_Meshes.push_back(std::make_pair(mesh,model_matrix));
		m_CurrentBatchSize++;
	}
}