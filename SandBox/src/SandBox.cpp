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

SandBox::SandBox(const char* title, float_t w, float_t h, bool resizeble)
	: Base::windowing::Window(title, w, h, resizeble)
{
}

SandBox::~SandBox()
{
}

void SandBox::OnAttach() //Called before the game loop starts
{
	m_Scene = Base::MakeScope<Base::Scene>(); //Create scene

	m_Camera = m_Scene->CreateEntity("Main2D_Camera"); //Create camera entity

	m_Entitys["Quad"] = m_Scene->CreateEntity("Quad"); //Create the Quad entity
	m_Entitys["Quad"].AddComponent<Base::SpriteComponent>(Color::Base_Color); //Add sprite (solid color)

	auto& Camera_Transform = m_Camera.GetComponent<Base::TransformComponent>();
	auto& Camera_comp = m_Camera.AddComponent<Base::CameraComponent>();

	Camera_comp.Camera.SetViewportSize(Base::WindowProps().width, Base::WindowProps().height);

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
}

void SandBox::Dispose()
{

}

void SandBox::OnResize(const Base::ResizeArgs& args)
{
	if (Base::WindowProps().minimized)
		return;
	auto& Camera_comp = m_Camera.GetComponent<Base::CameraComponent>();
	Camera_comp.Camera.SetViewportSize(args.new_w, args.new_h);
}
