#pragma once
#include "Base/Base.h"
#include "Base.h"
#include "scene/EditorCamera.h"
#include <string>
#include <unordered_map>
class TestLayer : public Base::Layer
{
private:
	Base::Scope<Base::Scene> m_Scene;
	Base::EditorCamera m_EditorCamera;
	Base::Entity m_Camera;
	std::unordered_map<std::string, Base::Entity> m_Entitys;
public:
	TestLayer(const std::string& name = "TestLayer");

	void OnAttach() override;
	void OnUpdate(UpdateArgs args) override;
	void OnDetach() override;
	void OnEvent(Base::Event& e) override;

	bool OnWindowResize(Base::WindowResizeEvent& e);
	bool OnMouseScroll(Base::MouseScrollEvent& e);
};

