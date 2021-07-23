#pragma once
#include "render/Texture.h"
#include "colision/Colisions.h"
#include "args/VirtualArgs.h"
#include "ent/Animator.h"
#include "render/ParticleSystem.h"
struct MoreArgs
{
	float Gravity;
	std::vector<en::Rect> Pipes;
};

class Piriquito
{
private: 
	en::render::Texture& m_Texture;
	en::Rect m_ColisionBox;
	bool m_Alive = false;
	float m_Rotation = 0.0f;
	float anim_vel = 35.0f;
	en::ett::Animator m_Animation;
	en::render::ParticleProps Props;
	en::render::ParticleSystem m_Particles;
public:
	Piriquito(en::render::Texture& texture);
	~Piriquito();

	void OnAttach(const en::AttachArgs& args);
	void OnUpdate(const en::UpdateArgs& args, const MoreArgs& more_args);
	void OnRender(const en::RenderArgs& args);
	void OnImGui(const en::ImGuiArgs& args);

	void Live();
	void Die();

	void SetTexture(en::render::Texture& new_texture);
	void SetFeatherTexture(en::render::Texture& new_texture);
	void SetRotation(float new_rotation);
	void SetAnimVel(float value)
	{
		anim_vel = value;
		m_Animation.SetDecrement(anim_vel);
		
	}
	inline float GetAnimVel() const { return anim_vel; }
	en::Rect& GetRect() { return m_ColisionBox; }
	//Return the desired colision box, may have a distorcion to the normal Rect or not (In this case, have)
	en::Rect GetColision() 
	{ 
		en::Rect t = m_ColisionBox;
		t.pos.x += 5.0f;
		return t;
	}

	bool IsAlive() const { return m_Alive; }
	bool GetRotation() const { return m_Rotation; }

};

