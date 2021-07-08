#include "Pipe.h"

Pipe::Pipe(en::render::Texture& texture)
	:m_Texture(texture),
	 m_ColisionBox({ {0.0f,0.0f}, {0.0f,0.0f}, {0.0f,0.0f} })
{
}

Pipe::~Pipe()
{
}

void Pipe::OnAttach(const en::AttachArgs& args)
{
}

void Pipe::OnUpdate(const en::UpdateArgs& args)
{
	constexpr glm::vec2 pipe_velocity = { 450.0f,0.0f };
	if (m_Alive)
	{
		m_ColisionBox.pos -= pipe_velocity * args.dt;
		
		if (m_ColisionBox.pos.x <= 0.0 - m_ColisionBox.size.x)
			Despawn();
	}
}

void Pipe::OnRender(const en::RenderArgs& args)
{
	float rotation = 0.0f;
	if (m_SRender)
	{
		if (invert)
			rotation = 180.0f;
		args.render.DrawQuad(m_ColisionBox.pos, m_ColisionBox.size, m_Texture, 2.0f, {1.0f,1.0f,1.0f,1.0f}, glm::radians(rotation));
		//args.render.DrawOutLineQuad(m_ColisionBox.pos, m_ColisionBox.size, {1.0f,0.0f,0.0f,1.0f}, 3.0f, glm::radians(rotation));
	}
}

void Pipe::OnImGui(const en::ImGuiArgs& args)
{
}

void Pipe::SetTexture(en::render::Texture& texture)
{
	m_Texture = texture;
}

void Pipe::Spawn(const en::Rect& pos)
{
	m_ColisionBox = const_cast<en::Rect&>(pos);
	m_SRender = true;
	m_Alive = true;
}

void Pipe::Despawn()
{
	//Go offscreen
	m_SRender = false;
	m_Alive = false;
	m_ColisionBox.pos = { -500.0f,-500.0f };
}
