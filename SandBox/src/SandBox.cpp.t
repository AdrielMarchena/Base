#include "SandBox.h"

#include "utils/Files.h"
#include "scene/Components.h"
#include "scene/CameraScript.h"
#include "utils/RandomList.h"

#include "scene/SceneSerializer.h"

#include "imgui.h"

#include "utils/Instrumentor.h"
#include "ImGuizmo.h"
#include "glm/gtc/type_ptr.hpp"




/*void ExLayer::OnAttach() //Called before the game loop starts
{
	m_Scene = Base::MakeRef<Base::Scene>(); //Create scene

	editor_camera = Base::EditorCamera(45.0f, m_Specs.width / m_Specs.height, 0.01f, 1000.0f);
	m_Scene->OnViewPortResize(m_Specs.width, m_Specs.height);
	editor_camera.SetViewportSize(m_Specs.width, m_Specs.height);
	Base::SceneSerializer serializer(m_Scene);
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

	Camera_comp.Camera.SetViewportSize(m_Specs.width, m_Specs.height);

	auto& Camera_Script = m_Camera.AddComponent<Base::NativeScriptComponent>(); 
	Camera_Script.Bind<Base::OrthoCameraScript>(); 
	m_Scene->StartNativeScript(m_Camera); 
	m_Scene->AwakeNativeScript(m_Camera);
}

void ExLayer::OnUpdate(const Base::UpdateArgs& args) //Called each frame
{
	using kb = Base::input::Keyboard;
	using ms = Base::input::Mouse;

	editor_camera.OnUpdate(args);
	if (!Base::WindowProps().minimized) 
		m_Scene->OnUpdateEditor(args,editor_camera);
}


void ExLayer::OnImGui()
{
	BASE_PROFILE_FUNCTION();
	ImGui::Begin("Framebuffer and post effects");

	static float frame_buff_scalor = 1.0f;
	if (ImGui::SliderFloat("Framebuffer scalor", &frame_buff_scalor, 0.05f, 1.0f))
		m_Scene->SetFramebufferScaler(frame_buff_scalor);

	if (ImGui::Button("none"))
		m_Scene->SetPostEffect("none");

	for (auto& efx : m_Scene->GetPostEffects())
	{
		if(ImGui::Button(efx.first.c_str()))
			m_Scene->SetPostEffect(efx.first);
	}
	///auto& color = m_Entitys["Quad"].GetComponent<Base::SpriteComponent>();
	//ImGui::SliderFloat3("Quad Color", &color.Color.r,0.0f,1.0f);

	ImGui::End();

	/*ImGui::Begin("Quad Physic");

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
			
			static std::string lab = "VSync = " + std::string(GetVSync() ? "Enabled" : "Disabled");
			if(ImGui::MenuItem(lab.c_str(), ""))
			{
				SetVSync(!GetVSync());
				std::string lab = "VSync = " + std::string(GetVSync() ? "Enabled" : "Disabled");
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Physics"))
		{
			if (ImGui::MenuItem("Start Simulation"))
				m_Scene->RuntimeInit();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("Save"))
			{
				Base::SceneSerializer serializer(m_Scene);
				serializer.Serialize("assets/scenes/scene1.base");
			}
				
			ImGui::EndMenu();
		}

	ImGui::EndMainMenuBar();
	}
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Gizmo");
	
	//auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	//auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	//auto viewportOffset = ImGui::GetWindowPos();
	//m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	//m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
	 
	ImGui::Image(reinterpret_cast<void*>(m_Scene->GetFramebufferImage()), ImVec2(m_Specs.width, m_Specs.height), ImVec2{ 0,1 }, ImVec2{ 1,0 });

	//Gizmo
	Base::Entity selected = m_Entitys["Platform"];
	if (selected)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		auto& tc = selected.GetTransform();
		auto transform = tc.GetTransform();

		bool snap = Base::input::Mouse::isPress(BASE_KEY_LEFT_CONTROL);
		float snapValue = 45.5f;

		float snapValues[3] = { snapValue, snapValue, snapValue };

		ImGuizmo::Manipulate(glm::value_ptr(editor_camera.GetViewMatrix()), glm::value_ptr(editor_camera.GetProjection()),
			ImGuizmo::OPERATION::ROTATE, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

		if (ImGuizmo::IsUsing())
		{
			glm::vec3  translation, rotation, scale;
			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), &translation.x, &rotation.x, &scale.x);

			glm::vec3 deltaRotation = rotation - tc.Rotation;
			tc.Translation = translation;
			tc.Rotation += deltaRotation;
			tc.Scale = scale;
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();
}

void ExLayer::Dispose()
{
	m_Scene->RuntimeStop();
}

void ExLayer::OnResize(const Base::ResizeArgs& args)
{
	if (Base::WindowProps().minimized)
		return;
	m_Scene->OnViewPortResize(args.new_w, args.new_h);
	editor_camera.SetViewportSize(args.new_w, args.new_h);
}

void ExLayer::OnMouseAction(const Base::MouseArgs& args)
{
	editor_camera.OnMouseScroll(args);
}*/
