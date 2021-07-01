#pragma once
#include "render/Texture.h"
#include "colision/Colisions.h"
#include "args/VirtualArgs.h"

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
	void SetRotation(float new_rotation);

	en::Rect& GetRect() { return m_ColisionBox; }

	bool IsAlive() const { return m_Alive; }
	bool GetRotation() const { return m_Rotation; }

};

