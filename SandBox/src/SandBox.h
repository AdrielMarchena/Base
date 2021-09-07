#pragma once
#include "Window.h"
#include "scene/Entity.h"
#include "scene/Scene.h"
#include "render/Camera.h"
class SandBox : public en::windowing::Window
{
private:
	en::Entity m_Camera;
	std::vector<en::Entity> m_Balls;
	std::unique_ptr<en::Scene> m_Scene;

public:
	SandBox();
	~SandBox();

	virtual void OnAttach() override;
	virtual void OnUpdate(const en::UpdateArgs& args) override;
	virtual void OnRender() override;
	virtual void OnImGui() override;
	virtual void Dispose() override;
};

