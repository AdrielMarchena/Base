#include "SandBox.h"

#include "utils/Files.h"

SandBox::SandBox()
	:Base::windowing::Window()
{
}

SandBox::~SandBox()
{
}

void SandBox::OnAttach()
{
	m_Textures = Base::render::Texture::LoadAsyncTexture(Base::utils::Files::GetPairText("images/"));
	
	m_Scene->SceneBegin();
	Window::OnAttach();
}

void SandBox::OnUpdate(const Base::UpdateArgs& args)
{
	m_Scene->OnUpdate(args);
}

void SandBox::OnRender()
{
}

void SandBox::OnImGui()
{
}

void SandBox::Dispose()
{
	m_Scene->SceneEnd();
}
