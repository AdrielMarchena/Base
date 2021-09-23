#pragma once

#include "Window.h"
#include "Cell.h"
#include "scene/Scene.h"
#include "scene/Entity.h"
class Game: public Base::windowing::Window
{
protected:
	Base::Scope<Base::Scene> m_Scene;
	Base::Entity m_Camera;
	Base::Entity m_Map;
	Base::Entity m_TestQuad;

public:
	Game();
	~Game();

	virtual void OnAttach() override;
	virtual void OnUpdate(const Base::UpdateArgs& args) override;
	virtual void OnImGui() override;
	virtual void Dispose() override;

	virtual void OnResize(const Base::ResizeArgs& args) override;
};

