#pragma once
#include "Base/Base.h"
#include "Window.h"
#include "scene/Entity.h"
#include "scene/Scene.h"
#include "render/Camera.h"
#include "ResourceManager.h"
class SandBox : public Base::windowing::Window
{
private:
	Base::Entity m_Camera;
	Base::Entity m_TexQuad;
	Base::Scope<Base::Scene> m_Scene;
	ResourceManager<Base::render::Texture> m_Textures;
public:
	SandBox();
	~SandBox();

	virtual void OnAttach() override;
	virtual void OnUpdate(const Base::UpdateArgs& args) override;
	virtual void OnRender() override;
	virtual void OnImGui() override;
	virtual void Dispose() override;
};

