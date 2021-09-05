#include "SandBox.h"

#include "scene/Components.h"

SandBox::SandBox()
	:en::windowing::Window()
{
}

SandBox::~SandBox()
{
}

void SandBox::OnAttach()
{
	m_Scene = std::make_unique<en::Scene>();
	m_Ball = m_Scene->CreateEntity("Ball");
	m_Ball.AddComponent<en::SpriteComponent>(glm::vec4(1.0f,1.0f,0.1f,1.0f));
	auto& Ball_position = m_Ball.GetComponent<en::TransformComponent>();

	Ball_position = glm::translate(glm::mat4(1.0f), { 50.0f,50.0f,1.0f })
		* glm::scale(glm::mat4(1.0f), { 50.0f, 50.0f, 1.0f });

	/*Ball_position = glm::translate(glm::mat4(1.0f), { 50.0f,50.0f,1.0f }) *
		glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 0, 1));*/
	
	glm::mat4 camera_transform = glm::translate(glm::mat4(1.0f), { 0.0f,0.0f,0.0f }) *
		glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 0, 1));

	m_Camera = m_Scene->CreateEntity("Main_Camera");
	m_Camera.AddComponent<en::CameraComponent>(camera_transform);
	
	Window::OnAttach();
}

void SandBox::OnUpdate(const en::UpdateArgs& args)
{
	m_Scene->OnUpdate(args);
	Window::OnUpdate(args);
}

void SandBox::OnRender()
{
	//en::render::Render2D::DrawCircle({ 50.0f,50.0f,1.0f }, 50.0f, { 1.0f,1.0f,0.2f,1.0f });
	Window::OnRender();
}

void SandBox::OnImGui()
{
	Window::OnImGui();
}

void SandBox::Dispose()
{
	Window::Dispose();
}
