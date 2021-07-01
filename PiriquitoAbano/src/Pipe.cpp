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
	if (m_SRender)
	{
		m_ColisionBox.pos -= pipe_velocity * args.dt;
		
		if (m_ColisionBox.pos.x <= 0.0 - m_ColisionBox.size.x)
			Despawn();
	}
}

void Pipe::OnRender(const en::RenderArgs& args)
{
	glm::vec2 pos = m_ColisionBox.pos;
	glm::vec2 size = m_ColisionBox.size;
	if (m_SRender)
	{
		if (invert)
		{
			pos =
			{
				pos.x, pos.y + size.y
			};
			size =
			{
				size.x, -size.y
			};
		}
		args.render.DrawQuad(pos, size, m_Texture, 2.0f);
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
}

void Pipe::Despawn()
{
	//Go offscreen
	m_SRender = false;
	m_ColisionBox.pos = { -500.0f,-500.0f };
}
