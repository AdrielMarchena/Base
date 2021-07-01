#pragma once

#include "render/Texture.h"
#include "render/render2D.h"
#include "colision/Colisions.h"
#include "args/VirtualArgs.h"

class Pipe
{
private:
	en::Rect m_ColisionBox;
	en::render::Texture& m_Texture;
	//Also to see if need to check colision
	bool m_SRender = true;
public:
	bool invert = false;
	Pipe(en::render::Texture& texture);
	~Pipe();
	void OnAttach(const en::AttachArgs& args);
	void OnUpdate(const en::UpdateArgs& args);
	void OnRender(const en::RenderArgs& args);
	void OnImGui(const en::ImGuiArgs& args);

	en::Rect GetRect() const { return m_ColisionBox; }

	void SetTexture(en::render::Texture& texture);

	bool IsRender()const { return m_SRender; }

	void Spawn(const en::Rect& pos);
	void Despawn();
};

