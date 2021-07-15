#include "Pipe.h"

Pipe::Pipe(en::render::Texture& texture)
	:m_HeadTexture(texture),
	 m_BodyTexture(texture),
	 m_ColisionBox({ {0.0f,0.0f}, {0.0f,0.0f}, {450.0f,0.0f} })
{
	m_SRender = false;
	m_Alive = false;
	pointPipe = false;
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
		m_ColisionBox.pos -= m_ColisionBox.velocity * args.dt;
		
		if (m_ColisionBox.pos.x <= 0.0 - m_ColisionBox.size.x)
			Despawn();
	}
}

void Pipe::OnRender(const en::RenderArgs& args)
{
	float rotation = 0.0f;
	float h = m_ColisionBox.size.y / 64.0f;
	constexpr glm::vec2 size = { 64.0f,64.0f };
	if (m_SRender)
	{
 		glm::vec2 current_pos = {m_ColisionBox.pos.x,m_ColisionBox.size.y};
		if (!invert)
		{
			current_pos.y = m_ColisionBox.pos.y;
			args.render.DrawQuad(m_ColisionBox.pos, size, m_HeadTexture, 2.0f, { 1.0f,1.0f,1.0f,1.0f }, glm::radians(rotation));
			current_pos.y += 64.0f;
			for (int i = 1; i < h; i++)
			{
				args.render.DrawQuad(current_pos, size, m_BodyTexture, 2.0f, { 1.0f,1.0f,1.0f,1.0f }, glm::radians(rotation));
				current_pos.y += 64.0f;
			}
		}
		else
		{
			rotation = 180.0f;
			args.render.DrawQuad(m_ColisionBox.pos, size, m_HeadTexture, 2.0f, { 1.0f,1.0f,1.0f,1.0f }, glm::radians(rotation));
			current_pos.y -= 64.0f*2;
			for (int i = 1; i < h; i++)
			{
				args.render.DrawQuad(current_pos, size, m_BodyTexture, 2.0f, { 1.0f,1.0f,1.0f,1.0f }, glm::radians(rotation));
				current_pos.y -= 64.0f*2;
			}
		}
			
		args.render.DrawOutLineQuad(m_ColisionBox.pos, m_ColisionBox.size, {1.0f,0.0f,0.0f,1.0f}, 3.0f, glm::radians(rotation));
	}
}

void Pipe::OnImGui(const en::ImGuiArgs& args)
{
}

void Pipe::SetTexture(en::render::Texture& texture)
{
	m_HeadTexture = texture;
}

void Pipe::SetBodyTexture(en::render::Texture& texture)
{
	m_BodyTexture = texture;
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
	pointPipe = false;
	m_ColisionBox.pos = { -5000.0f,-5000.0f };
}
