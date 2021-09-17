#include "SandBox.h"

#include "utils/Files.h"
#include "scene/Components.h"

#include "imgui.h"

SandBox::SandBox()
	:Base::windowing::Window()
{
}

SandBox::~SandBox()
{
}

void SandBox::OnAttach()
{
	m_Scene = std::make_unique<Base::Scene>();
	m_Textures = Base::render::Texture::LoadAsyncTexture(Base::utils::Files::GetPairText("images/"));
	
	m_TexQuad = m_Scene->CreateEntity("Quad_1");
	m_TexQuad2 = m_Scene->CreateEntity("Quad_2");

	//m_TexQuad.AddComponent<Base::TextureComponent>(m_Textures["MuriloDude"]);
	m_TexQuad.AddComponent<Base::SpriteComponent>().Color = Color::Base_Color;
	m_TexQuad2.AddComponent<Base::TextureComponent>(m_Textures["test"]);
	m_TexQuad2.AddComponent<Base::CircleComponent>(0.5f);

	auto& t1 = m_TexQuad.GetComponent<Base::TransformComponent>().Transform;
	auto& t2 = m_TexQuad2.GetComponent<Base::TransformComponent>().Transform;

	BASE_XPOS(t1) = 0.0f;
	BASE_YPOS(t1) = 0.0f;
	BASE_ZPOS(t1) = -1.0f;
	t1 = glm::scale(t1, glm::vec3(1.0f, -1.0f, 1.0f));

	BASE_XPOS(t2) = 0.0f;
	BASE_YPOS(t2) = 0.0f;
	BASE_ZPOS(t2) = 0.5f;
	//t2 = glm::scale(t2, glm::vec3(1.0f, -1.0f, 1.0f));

	glm::mat4 camera = glm::translate(glm::mat4(1.0f), { 0.0f,0.0f,0.0f }) *
		glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 0, 1));

	glm::mat4 camera_transform = glm::translate(glm::mat4(1.0f), { 0.0f,0.0f,0.0f }) *
		glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 0, 1));

	BASE_XPOS(camera_transform) = 0.0f;
	BASE_YPOS(camera_transform) = 0.0f;
	BASE_ZPOS(camera_transform) = 1.0f;

	m_Camera = m_Scene->CreateEntity("Main_Camera");
	m_Camera.AddComponent<Base::CameraComponent>(camera_transform).Primary = true;
	m_Camera.GetComponent < Base::TransformComponent>().Transform = camera_transform;

	m_Scene->SceneBegin();
	Window::OnAttach();
}

void SandBox::OnUpdate(const Base::UpdateArgs& args)
{
	if(!Base::WindowProps::minimized)
		m_Scene->OnUpdate(args);
}

void SandBox::OnRender()
{
}

void SandBox::OnImGui()
{
	ImGui::Text("SandBox Debug");

	ImGui::SliderFloat3("Camera position", BASE_XPOS(&m_Camera.GetComponent<Base::TransformComponent>().Transform), -2.0f, 2.0f);
	ImGui::SliderFloat3("Ball position", BASE_XPOS(&m_TexQuad2.GetComponent<Base::TransformComponent>().Transform), -2.0f, 2.0f);
	ImGui::SliderFloat("Ball radius", &m_TexQuad2.GetComponent<Base::CircleComponent>().Radius, 0.0f, 1.0f);
	ImGui::SliderFloat3("Dude position", BASE_XPOS(&m_TexQuad.GetComponent<Base::TransformComponent>().Transform), -2.0f, 2.0f);
}

void SandBox::Dispose()
{
	m_Scene->SceneEnd();
	m_Textures.Map([](Base::Ref<Base::render::Texture> sptr)
	{
		sptr->Dispose();
	});
}

void SandBox::OnResize(const Base::ResizeArgs& args)
{
}
