#pragma once

#include <unordered_map>
#include <vector>
#include "Base/Base.h"
#include "Window.h"
#include "scene/Entity.h"
#include "scene/Scene.h"
#include "render/Camera.h"
#include "ResourceManager.h"

#include "render/3d/Render3D.h"
#include "render/3d/Model.h"
#include "scene/SceneCamera.h"
#include "scene/Components.h"

#include <unordered_map>
/* Example class */
class Example1 : public Base::windowing::Window
{
private:
	Base::Entity m_Camera;
	Base::Entity m_Camera3D;
	std::unordered_map<std::string, Base::Entity> m_Entitys;
	std::vector<Base::Entity> m_Model;

	Base::Scope<Base::Scene> m_Scene;
public:
	Example1();
	Example1(const char* title, float_t w, float_t h, bool resizeble,bool fullscreen);
	Example1(const Base::windowing::WindowSpecification& specs);
	~Example1();

	virtual void OnAttach() override;
	virtual void OnUpdate(const Base::UpdateArgs& args) override;
	virtual void OnRender() override;
	virtual void OnImGui() override;
	virtual void Dispose() override;

	virtual void OnResize(const Base::ResizeArgs& args) override;
	virtual void OnMouseAction(const Base::MouseArgs& args) override {}
	virtual void OnKeyboardAction(const Base::KeyboardArgs& args) override {}
};

