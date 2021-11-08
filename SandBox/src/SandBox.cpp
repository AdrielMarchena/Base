#include "SandBox.h"

#include "utils/Files.h"
#include "scene/Components.h"
#include "scene/CameraScript.h"
#include "imgui.h"
#include "utils/RandomList.h"
#include "render/gl/Gl_Commands.h"

SandBox::SandBox()
	:Base::windowing::Window()
{
}

SandBox::SandBox(const char* title, float_t w, float_t h, bool resizeble, bool fullscreen)
	: Base::windowing::Window(title, w, h, resizeble,fullscreen)
{
}

SandBox::SandBox(const Base::windowing::WindowSpecification& specs)
	: Base::windowing::Window(specs)
{
}

SandBox::~SandBox()
{
}

void SandBox::OnAttach() //Called before the game loop starts
{
	m_Scene = Base::MakeScope<Base::Scene>(); //Create scene

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
		APP_INFO("Platform uuid: {0}", m_Entitys["Platform2"].GetID());
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
		APP_INFO("Quad uuid: {0}", m_Entitys["Quad"].GetID());

		auto& quad_rbody = m_Entitys["Quad"].AddComponent<Base::RigidBody2DComponent>();
		auto& quad_bcol = m_Entitys["Quad"].AddComponent<Base::BoxColider2DComponent>();

		quad_rbody.Type = Base::RigidBody2DComponent::BodyType::Dynamic;
	}

	auto& Camera_Transform = m_Camera.GetComponent<Base::TransformComponent>();
	auto& Camera_comp = m_Camera.AddComponent<Base::CameraComponent>();

	Camera_comp.Camera.SetViewportSize(m_Specs.width, m_Specs.height);

	auto& Camera_Script = m_Camera.AddComponent<Base::NativeScriptComponent>(); 
	Camera_Script.Bind<Base::OrthoCameraScript>(); 
	m_Scene->StartNativeScript(m_Camera); 
	m_Scene->AwakeNativeScript(m_Camera); 
}

void SandBox::OnUpdate(const Base::UpdateArgs& args) //Called each frame
{
	using kb = Base::input::Keyboard;
	using ms = Base::input::Mouse;

	if (!Base::WindowProps().minimized) 
		m_Scene->OnUpdate(args);
}

void SandBox::OnRender()
{
}

void SandBox::OnImGui() 
{
	ImGui::Begin("Framebuffer and post effects");

	static float frame_buff_scalor = 1.0f;
	if (ImGui::SliderFloat("Framebuffer scalor", &frame_buff_scalor, 0.05f, 1.0f))
		m_Scene->SetFrameBuff(m_Specs.width, m_Specs.height, frame_buff_scalor);

	if (ImGui::Button("none"))
		m_Scene->SetPostEffect("none");

	for (auto& efx : m_Scene->GetPostEffects())
	{
		if(ImGui::Button(efx.first.c_str()))
			m_Scene->SetPostEffect(efx.first);
	}
	auto& color = m_Entitys["Quad"].GetComponent<Base::SpriteComponent>();

	ImGui::SliderFloat3("Quad Color", &color.Color.r,0.0f,1.0f);

	ImGui::End();

	ImGui::Begin("Quad Physic");

	if (ImGui::Button("Start Simulation"))
	{
		m_Scene->RuntimeInit();
	}

	auto& quad_rbody = m_Entitys["Quad"].GetComponent<Base::RigidBody2DComponent>();
	auto& quad_bcol = m_Entitys["Quad"].GetComponent<Base::BoxColider2DComponent>();

	//ImGui::Text("Quad Rigid Body");
	//if (ImGui::Checkbox("FixedRotation", &quad_rbody.FixedRotation))
	//{
	//}

	ImGui::Text("Quad Box Colider");
	ImGui::SliderFloat2("Size", &quad_bcol.Size.x, 0.0f, 1.0f);
	ImGui::SliderFloat("Density", &quad_bcol.Density, 0.0f, 1.0f);
	ImGui::SliderFloat("Friction", &quad_bcol.Friction, 0.0f, 1.0f);
	ImGui::SliderFloat("Restitution", &quad_bcol.Restitution, 0.0f, 1.0f);
	ImGui::SliderFloat("RestitutionThreshold", &quad_bcol.RestitutionThreshold, 0.0f, 1.0f);

	ImGui::End();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("General"))
		{
			static bool quit_selected = false;
			ImGui::MenuItem("Quit", "", &quit_selected); if (quit_selected) CloseWindow();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Physics"))
		{
			if (ImGui::MenuItem("Start Simulation"))
				m_Scene->RuntimeInit();
			ImGui::EndMenu();
		}

	ImGui::EndMainMenuBar();
	}
}

void SandBox::Dispose()
{
	m_Scene->RuntimeStop();
}

void SandBox::OnResize(const Base::ResizeArgs& args)
{
	if (Base::WindowProps().minimized)
		return;
	auto& Camera_comp = m_Camera.GetComponent<Base::CameraComponent>();
	Camera_comp.Camera.SetViewportSize(args.new_w, args.new_h);
}
