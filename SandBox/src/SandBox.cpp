#include "SandBox.h"

#include "utils/Files.h"
#include "scene/Components.h"
#include "scene/CameraScript.h"
#include "imgui.h"
#include "utils/RandomList.h"
#include "render/gl/Gl_Commands.h"

//#define USING_3DCAMERA_EXAMPLE

float Fov = 45.0f;
glm::vec3 Pos;
glm::vec3 Scale;
static inline bool Preprare3DCamera(Base::Scope<Base::Scene>& scene,Base::Entity& camera)
{
#if defined USING_3DCAMERA_EXAMPLE
	camera = scene->CreateEntity("Main3D_Camera");

	auto& Camera_Transform = camera.GetComponent<Base::TransformComponent>();
	auto& Camera_comp = camera.AddComponent<Base::CameraComponent>();
	auto& Camera_Script = camera.AddComponent<Base::NativeScriptComponent>();

	Camera_Script.Bind<Base::PerspectiveScript>();

	Camera_comp.Camera.SetPerspective(glm::radians(45.0f), -10.0f, 10.0f);
	Camera_comp.Camera.SetViewportSize(Base::WindowProps().width, Base::WindowProps().height);
	scene->StartNativeScript(camera);
	scene->AwakeNativeScript(camera);

	return true;
#endif
	return false;
}

SandBox::SandBox()
	:Base::windowing::Window()
{
}

SandBox::~SandBox()
{
}

void SandBox::OnAttach()
{
	m_Scene = Base::MakeScope<Base::Scene>();

	auto texture = Base::render::Texture::CreateTexture("images/test.PNG");
	if (texture->GetId()) APP_INFO("texture using 'test.PNG' created!");

	m_Camera = m_Scene->CreateEntity("Main2D_Camera");

	m_Entitys["Circle"] = m_Scene->CreateEntity("Circle");
	m_Entitys["Circle"].AddComponent<Base::CircleComponent>();
	m_Entitys["Circle"].AddComponent<Base::TextureComponent>(texture);

	m_Entitys["Quad"] = m_Scene->CreateEntity("Quad");
	m_Entitys["Quad"].AddComponent<Base::SpriteComponent>(Color::Green);
	m_Entitys["Quad"].GetComponent<Base::TransformComponent>().Scale = glm::vec3(1.0f);

	
	auto& Camera_Transform = m_Camera.GetComponent<Base::TransformComponent>();
	auto& Camera_comp = m_Camera.AddComponent<Base::CameraComponent>();
	auto& Camera_Script = m_Camera.AddComponent<Base::NativeScriptComponent>();

	Camera_comp.Camera.SetViewportSize(Base::WindowProps().width, Base::WindowProps().height);	

	if (Preprare3DCamera(m_Scene, m_Camera))
	{
		APP_INFO("3D Camera script enabled");
		auto sphere = Base::Model::CreateModel("resources/models/sphere/15977_Sphere_with_Grid_v1.obj");
		if (texture->GetId()) APP_INFO("model using 'resources/models/sphere/15977_Sphere_with_Grid_v1.obj' created!");
		m_Entitys["Sphere"] = m_Scene->CreateEntity("Sphere");
		m_Entitys["Sphere"].AddComponent<Base::ModelComponent>(sphere);

		this->HideCursor();
	}
	else // Only bind script on 2DCamera if there is no 3D camera
	{
		Camera_Script.Bind<Base::OrthoCameraScript>();
		m_Scene->StartNativeScript(m_Camera);
		m_Scene->AwakeNativeScript(m_Camera);
	}
}

void SandBox::OnUpdate(const Base::UpdateArgs& args)
{
	using kb = Base::input::Keyboard;
	using ms = Base::input::Mouse;

#if defined USING_3DCAMERA_EXAMPLE
	if (kb::isClicked(BASE_KEY_H))
	{
		auto& c = m_Camera3D.GetComponent<Base::NativeScriptComponent>();
		((Base::PerspectiveScript*)&c.Instance)->mouse_is_hide = true;
		this->HideCursor();
	}
	if (kb::isClicked(BASE_KEY_U))
	{
		auto& c = m_Camera3D.GetComponent<Base::NativeScriptComponent>();
		((Base::PerspectiveScript*)&c.Instance)->mouse_is_hide = false;
		this->UnhideCursor();
	}
#endif
	if(!Base::WindowProps().minimized)
		m_Scene->OnUpdate(args);
}

void SandBox::OnRender()
{
}

void SandBox::OnImGui()
{

	auto& circ = m_Entitys["Circle"].GetComponent<Base::CircleComponent>();
	auto& quad = m_Entitys["Quad"].GetComponent<Base::TransformComponent>();
	ImGui::Begin("Circle");

	if(ImGui::SliderFloat("Radius", &circ.Radius,0.0f,10.0f))
		quad.Scale = glm::vec3(circ.Radius);
	ImGui::SliderFloat("Thickness", &circ.Thickness, 0.0f, 1.0f);
	ImGui::SliderFloat("Fade", &circ.Fade, 0.0f, 10.0f);

	ImGui::SliderFloat3("Quad Pos", &quad.Translation.x, -10.0f, 10.0f);

	ImGui::End();

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
#if defined USING_3DCAMERA_EXAMPLE
	auto& Camera3D = m_Camera3D.GetComponent<Base::CameraComponent>();
	Camera3D.Camera.SetViewportSize(args.new_w, args.new_h);
#endif
}
