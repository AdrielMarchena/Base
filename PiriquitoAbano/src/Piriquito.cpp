#include "Piriquito.h"

#include <algorithm>
#include "imgui.h"

Piriquito::Piriquito(std::shared_ptr<en::render::Texture> texture)
	:m_Texture(texture), 
	 m_ColisionBox({ {0.0f,0.0f}, {texture->GetSize()}, {0.0f,0.0f} })
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
		{50.0f,55.0f},  //Size
		{0.0f,0.0f}		//Velocity
	};

	Props = en::render::ParticleProps::Effects[0];
	Props.Gravity = 1.8f;
	Props.TexturePtr = nullptr;
	m_Particles = en::render::ParticleSystem(75);
	m_Rotation = -90.0f;
}

void Piriquito::OnUpdate(const en::UpdateArgs& args, const MoreArgs& more_args)
{
	static const glm::vec2 terminal_neg_velocity = { 0.0f,-350.0f };
	static const glm::vec2 up_velocity = { 0.0f,250.0f };
		
	if (m_Rotation < -99.0f) // Add some extra gravity if the bird is turn down
	{
		m_ColisionBox.velocity.y -= more_args.Gravity * 1.15f;
		if (m_ColisionBox.velocity.y <= terminal_neg_velocity.y * 1.15f)
			m_ColisionBox.velocity.y = terminal_neg_velocity.y * 1.15f;
	}
	else
	{
		m_ColisionBox.velocity.y -= more_args.Gravity;
		if (m_ColisionBox.velocity.y <= terminal_neg_velocity.y)
			m_ColisionBox.velocity.y = terminal_neg_velocity.y;
	}
		
	//else
		//m_ColisionBox.velocity += dec_velocity * args.dt;

	if (args.mouse.isClicked(GLFW_MOUSE_BUTTON_1) || args.keyboard.isClicked(GLFW_KEY_SPACE))
	{
		m_ColisionBox.velocity = up_velocity;
		Props.Position = m_ColisionBox.pos + (m_ColisionBox.size / 2.0f);
		for(int i = 0; i < 4; i++)
			m_Particles.Emit(Props);
	}
	m_ColisionBox.pos += m_ColisionBox.velocity * args.dt;
	m_Rotation += (m_ColisionBox.velocity.y * args.dt) / 2;
	m_Rotation = std::clamp(m_Rotation, -100.0f, -20.0f);

	//TODO: remove this logic
	m_Animation.BeginUpdate();
}

void Piriquito::OnRender(const en::RenderArgs& args)
{
	m_Particles.OnUpdate(args.dt);
	glm::vec2 rotate_pos = m_ColisionBox.pos;
	glm::vec2 rotate_size = {64.0f,64.0f};
	rotate_pos.y += rotate_size.y;
	rotate_size.y = -rotate_size.y;
	auto& current_subt = m_Animation.Run(args.dt);
	args.render.DrawQuad(rotate_pos, rotate_size, current_subt, 2.0f ,glm::radians(m_Rotation));
	const auto b = GetColision();
	m_Particles.OnRender(args);

	//TODO: remove this logic
	m_Animation.StopUpdate();
}

void Piriquito::OnImGui(const en::ImGuiArgs& args)
{
	ImGui::Text("Piriquito");
	if (ImGui::SliderFloat("Animation Velocity", &anim_vel,0.5f,50.0f))
		m_Animation.SetDecrement(anim_vel);
}

void Piriquito::Live()
{
	m_Alive = true;
	m_Animation.BeginUpdate();
}

void Piriquito::Die()
{
	auto t = Props.LifeTime;
	Props.LifeTime = 2.5f;
	//Animation for death maybe?
	for (int i = 0; i < 50; i++)
		m_Particles.Emit(Props);
	m_Animation.StopUpdate();
	Props.LifeTime = t;
	SetAnimVel(35.0f);
	m_Animation.Reset();
	m_Alive = false;
}

void Piriquito::SetTexture(std::shared_ptr<en::render::Texture> new_texture)
{
	if (new_texture->GetId())
	{
		m_Texture = new_texture;
		m_ColisionBox.size = new_texture->GetSize();
		en::ett::AnimationSpecs specs;
		specs.atlas = new_texture;
		specs.threshold = 25.0f;
		m_Animation.SetNewTexture(specs);
		m_Animation.SetDecrement(anim_vel);
		m_Animation.loop = true;
	}
}

void Piriquito::SetFeatherTexture(std::shared_ptr<en::render::Texture> new_texture)
{
	Props.TexturePtr = new_texture;
}

void Piriquito::SetRotation(float new_rotation)
{
	m_Rotation = new_rotation;
}

