#pragma once

#include "Window.h"
#include "Cell.h"
#include "scene/Scene.h"
#include "scene/Entity.h"
#include <unordered_map>
#include <future>

class Game: public Base::windowing::Window
{
protected:
	Base::Scope<Base::Scene> m_Scene;
	Base::Entity m_Camera;
	Base::Entity m_Camera2D;
	Base::Entity m_Sprite;
	Base::Entity m_Map;
	Base::Entity m_Cube;
	std::unordered_map<std::string ,Base::Ref<Base::render::Texture>> m_Textures;

public:
	Game();
	~Game();

	virtual void OnAttach() override;
	virtual void OnUpdate(const Base::UpdateArgs& args) override;
	virtual void OnImGui() override;
	virtual void Dispose() override;

	virtual void OnResize(const Base::ResizeArgs& args) override;

	bool LoadAsync();
};

