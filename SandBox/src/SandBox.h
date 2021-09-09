#pragma once
#include "Window.h"
#include "scene/Entity.h"
#include "scene/Scene.h"
#include "render/Camera.h"
#include "ResourceManager.h"
class SandBox : public en::windowing::Window
{
private:
	en::Entity m_Camera;
	std::vector<en::Entity> m_Balls;
	en::Entity m_TexQuad;
	std::unique_ptr<en::Scene> m_Scene;
	ResourceManager<en::render::Texture> m_Textures;
public:
	SandBox();
	~SandBox();

	virtual void OnAttach() override;
	virtual void OnUpdate(const en::UpdateArgs& args) override;
	virtual void OnRender() override;
	virtual void OnImGui() override;
	virtual void Dispose() override;
};

