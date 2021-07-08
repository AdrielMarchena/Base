#include "Piriquito.h"

#include <algorithm>

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
		{50.0f,300.0f}, //Position
		{64.0f,64.0f},  //Size
		{0.0f,0.0f}	//Velocity
	};
	m_Rotation = -90.0f;
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

	if (args.mouse.isClicked(GLFW_MOUSE_BUTTON_1) || args.keyboard.isClicked(GLFW_KEY_SPACE))
	{
		m_ColisionBox.velocity = up_velocity;
	}

	m_ColisionBox.pos += m_ColisionBox.velocity * args.dt;
	m_Rotation += (m_ColisionBox.velocity.y * args.dt) / 2;
	m_Rotation = std::clamp(m_Rotation, -100.0f, -20.0f);

}

void Piriquito::OnRender(const en::RenderArgs& args)
{
	glm::vec2 rotate_pos = m_ColisionBox.pos;
	glm::vec2 rotate_size = m_ColisionBox.size;
	rotate_pos.y += m_ColisionBox.size.y;
	rotate_size.y = -rotate_size.y;
	args.render.DrawQuad(rotate_pos, rotate_size, m_Texture, 2.0f, en::render::Color::White ,glm::radians(m_Rotation));
	//args.render.DrawOutLineQuad(rotate_pos, rotate_size, { 1.0f,0.0f,0.0f,1.0f }, 3.0f);
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

