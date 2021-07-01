#include "Piriquito.h"

Piriquito::Piriquito(en::render::Texture& texture)
	:m_Texture(texture), 
	 m_ColisionBox({ {0.0f,0.0f}, {texture.GetSize()}, {0.0f,0.0f} })
{
}
Piriquito::~Piriquito()
{
}

void Piriquito::OnAttach(const en::AttachArgs& args)
{
	m_ColisionBox =
	{
		{50.0f,300.0f + 64.0f}, //Position
		{64.0f,-64.0f},  //Size
		{0.0f,0.0f}	//Velocity
	};
}

void Piriquito::OnUpdate(const en::UpdateArgs& args, const MoreArgs& more_args)
{
	static const glm::vec2 terminal_neg_velocity = { 0.0f,-350.0f };
	static const glm::vec2 up_velocity = { 0.0f,250.0f };
	m_ColisionBox.velocity.y -= more_args.Gravity;
	
	if (m_ColisionBox.velocity.y <= terminal_neg_velocity.y)
		m_ColisionBox.velocity.y = terminal_neg_velocity.y;
	//else
		//m_ColisionBox.velocity += dec_velocity * args.dt;

	if (args.mouse.isAnyKey() || args.keyboard.isAnyKey())
	{
		m_ColisionBox.velocity = up_velocity;
	}

	m_ColisionBox.pos += m_ColisionBox.velocity * args.dt;

	for (auto& pipe : more_args.Pipes)
	{
		if (en::colision::Colide::RectVsRect(m_ColisionBox, pipe))
		{
			Die();
			break;
		}
	}
}

void Piriquito::OnRender(const en::RenderArgs& args)
{
	args.render.DrawQuad(m_ColisionBox.pos, m_ColisionBox.size, m_Texture, 2.0f, m_Rotation);
}

void Piriquito::OnImGui(const en::ImGuiArgs& args)
{
}

void Piriquito::Live()
{
	m_Alive = true;
}

void Piriquito::Die()
{
	//Animation for death maybe?

	m_Alive = false;
}

void Piriquito::SetTexture(en::render::Texture& new_texture)
{
	if (new_texture.GetId())
		m_Texture = new_texture;
}

void Piriquito::SetRotation(float new_rotation)
{
	m_Rotation = new_rotation;
}

