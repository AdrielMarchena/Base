#include "TestLayer.h"

#include "scene/Components.h"

TestLayer::TestLayer(const std::string& name)
	:Base::Layer(name)
{
}

void TestLayer::OnAttach()
{
	m_Scene = Base::MakeScope<Base::Scene>(); //Create scene

	int w = Base::WindowProps().width;
	int h = Base::WindowProps().height;

	m_EditorCamera = Base::EditorCamera(45.0f, w / h, 0.01f, 1000.0f);
	m_Scene->OnViewPortResize(w, h);
	m_EditorCamera.SetViewportSize(w, h);

	//Base::SceneSerializer serializer(m_Scene);
	//if (serializer.Deserialize("assets/scenes/scene1.base"))
	//	return;

	m_Camera = m_Scene->CreateEntity("Main2D_Camera"); //Create camera entity

	{
		m_Entitys["Platform"] = m_Scene->CreateEntity("Platform"); //Create the Quad entity
		m_Entitys["Platform"].AddComponent<Base::SpriteComponent>(Color::Green); //Add sprite (solid color)
		auto& plat_tranform = m_Entitys["Platform"].GetTransform();

		plat_tranform.Translation = { 0.0,-10.5, 0.0f };
		plat_tranform.Scale = { 20.0f, 1.0f, 1.0f };
		plat_tranform.Rotation = { 0.0f, 0.0f, 0.8f };

		auto& quad_rbody = m_Entitys["Platform"].AddComponent<Base::RigidBody2DComponent>();
		auto& quad_bcol = m_Entitys["Platform"].AddComponent<Base::BoxColider2DComponent>();

		quad_rbody.Type = Base::RigidBody2DComponent::BodyType::Static;
	}

	{
		m_Entitys["Platform2"] = m_Scene->CreateEntity("Platform2"); //Create the Quad entity
		m_Entitys["Platform2"].AddComponent<Base::SpriteComponent>(Color::Green); //Add sprite (solid color)
		auto& plat_tranform = m_Entitys["Platform2"].GetTransform();

		plat_tranform.Translation = { 0.0,-10.5, 0.0f };
		plat_tranform.Scale = { 20.0f, 1.0f, 1.0f };
		plat_tranform.Rotation = { 0.0f, 0.0f, -1.2f };

		auto& quad_rbody = m_Entitys["Platform2"].AddComponent<Base::RigidBody2DComponent>();
		auto& quad_bcol = m_Entitys["Platform2"].AddComponent<Base::BoxColider2DComponent>();

		quad_rbody.Type = Base::RigidBody2DComponent::BodyType::Static;
	}

	{
		m_Entitys["Quad"] = m_Scene->CreateEntity("Quad"); //Create the Quad entity
		m_Entitys["Quad"].AddComponent<Base::SpriteComponent>(Color::Base_Color); //Add sprite (solid color)

		auto& quad_rbody = m_Entitys["Quad"].AddComponent<Base::RigidBody2DComponent>();
		auto& quad_bcol = m_Entitys["Quad"].AddComponent<Base::BoxColider2DComponent>();

		quad_rbody.Type = Base::RigidBody2DComponent::BodyType::Dynamic;
	}

	auto& Camera_Transform = m_Camera.GetComponent<Base::TransformComponent>();
	auto& Camera_comp = m_Camera.AddComponent<Base::CameraComponent>();

	Camera_comp.Camera.SetViewportSize(w,h);

	auto& Camera_Script = m_Camera.AddComponent<Base::NativeScriptComponent>();
	Camera_Script.Bind<Base::OrthoCameraScript>();
	m_Scene->StartNativeScript(m_Camera);
	m_Scene->AwakeNativeScript(m_Camera);
}

void TestLayer::OnUpdate(UpdateArgs args)
{
	using kb = Base::input::Keyboard;
	using ms = Base::input::Mouse;

	if (!Base::WindowProps().minimized)
	{
		m_EditorCamera.OnUpdate(args);
		if (m_Runtime)
			m_Scene->OnUpdateRuntime(args);
		else
			m_Scene->OnUpdateEditor(args, m_EditorCamera);
	}
}

void TestLayer::OnDetach()
{
	m_Scene->RuntimeStop();
}

void TestLayer::OnEvent(Base::Event& e)
{
	Base::EventDispatcher disp(e);

	disp.Dispatch<Base::WindowResizeEvent>(BIND_EVENT_FN(TestLayer::OnWindowResize));
	disp.Dispatch<Base::MouseScrollEvent>(BIND_EVENT_FN(TestLayer::OnMouseScroll));
}

bool TestLayer::OnWindowResize(Base::WindowResizeEvent& e)
{
	m_Scene->OnViewPortResize(e.GetWidth(), e.GetHeight());
	m_EditorCamera.SetViewportSize(e.GetWidth(), e.GetHeight());
	return false;
}

bool TestLayer::OnMouseScroll(Base::MouseScrollEvent& e)
{
	m_EditorCamera.OnMouseScroll(e);
	return false;
}
