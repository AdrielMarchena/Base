#include "Game.h"
#include <corecrt_math.h>
#include "scene/Components.h"
#include "scene/CameraScript.h"
#include "imgui.h"
#include "render/3d/Render3D.h"
#include "utils/RandomList.h"

float scale_factor = 1.0f;

Game::Game()
	:Base::windowing::Window()
{
}

Game::Game(const char* title, float_t w, float_t h, bool resizeble, bool fullscreen)
	: Base::windowing::Window(title,w,h,resizeble,fullscreen)
{
}

Game::~Game()
{
}

void Game::OnAttach()
{
	m_Scene = Base::MakeScope <Base::Scene>();
	m_Map = m_Scene->CreateEntity("Map");

	m_Map.AddComponent<Base::TextureComponent>();
	auto& map_script = m_Map.AddComponent<Base::NativeScriptComponent>();
	
	map_script.Bind<MapScript>();

	m_Scene->StartNativeScript(m_Map);
	m_Scene->AwakeNativeScript(m_Map);

#ifdef BASE_USING_3D
	m_Map.GetComponent<Base::TransformComponent>().Translation = { 1000.f,1000.0f,1000.0f };
	m_Map.GetComponent<Base::TransformComponent>().Scale = { 0,00.0f,000.0f };
	m_Camera = m_Scene->CreateEntity("Main_Camera3D");

	auto& scp3D = m_Camera.AddComponent<Base::NativeScriptComponent>();
	{// 3D Camera
		scp3D.Bind<Base::PerspectiveScript>();
		auto& c = m_Camera.AddComponent<Base::CameraComponent>();
		auto& camera_transform = m_Camera.GetComponent<Base::TransformComponent>();

		c.Camera.SetPerspective(45.0f, 0.1f, 9000.0f);
		c.Camera.SetViewportSize(Base::WindowProps().width, Base::WindowProps().height);
	}

	auto& map_texture = m_Map.GetComponent<Base::TextureComponent>().Texture;

	int cube_qtd = 10;

#if defined BASE_RELEASE || BASE_DIST
	cube_qtd = 250;
#endif

	const float cube_spread = 0.001f;
	const float rotation_variation = 0.001f;
	const glm::vec3 cube_size_variation = glm::vec3(0.005f, 0.005f, 0.005f);
	auto cube_3dmodel = Base::Model::CreateModel("resources/models/cube/cube.obj");
	for (int i = 0; i < cube_qtd; i++)
	{
		const std::string tag_name = "Cube_" + std::to_string(i);
		m_Cubes[tag_name] = m_Scene->CreateEntity(tag_name);

		auto& cube_transform = m_Cubes[tag_name].GetComponent<Base::TransformComponent>();
		//Cube position
		float neg_faq_x = M_random() % 2 == 0 ? 1.0f : -1.0f;
		float neg_faq_y = M_random() % 2 == 0 ? 1.0f : -1.0f;
		float neg_faq_z = M_random() % 2 == 0 ? 1.0f : -1.0f;
		cube_transform.Translation =
		{
			(float)(M_random() << 2) * neg_faq_x * cube_spread * (i * 1.1f) ,
			(float)(M_random() << 2) * neg_faq_y * cube_spread * (i * 1.1f) ,
			(float)(M_random() << 2) * neg_faq_z * cube_spread * (i * 1.1f) 
		};

		cube_transform.Rotation =
		{
			glm::radians((float)(M_random() << 1) * rotation_variation),
			glm::radians((float)(M_random() << 1) * rotation_variation),
			glm::radians((float)(M_random() << 1) * rotation_variation)
		};

		auto v = (float)M_random();
		cube_transform.Scale =
		{
			cube_size_variation.x * v + (i * 0.01f),
			cube_size_variation.y * v + (i * 0.01f),
			cube_size_variation.z * v + (i * 0.01f)
		};

		auto& cube_model = m_Cubes[tag_name].AddComponent<Base::ModelComponent>(cube_3dmodel);
		cube_model.Model3D->OverrideTexture(map_texture);
	}

	m_Scene->StartNativeScript(m_Camera);

	HideCursor();
	((Base::PerspectiveScript*)scp3D.Instance)->mouse_is_hide = true;

	std::vector<std::string> faces;
	faces =
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Base::Render3D::SetSkyBox("shaders/CubeMap.glsl", faces);

#else
	m_Camera = m_Scene->CreateEntity("Main_Camera2D");
	auto& scp2D = m_Camera.AddComponent<Base::NativeScriptComponent>();
	{// 2D Camera
		scp2D.Bind<Base::OrthoCameraScript>();
		auto& c = m_Camera.AddComponent<Base::CameraComponent>();
		auto& camera_transform = m_Camera.GetComponent<Base::TransformComponent>();

		c.Camera.SetViewportSize(Base::WindowProps().width, Base::WindowProps().height);
	}
	m_Scene->StartNativeScript(m_Camera);
#endif
}

void Game::OnUpdate(const Base::UpdateArgs& args)
{
	if (Base::WindowProps().minimized)
		return;

#ifdef BASE_USING_3D
	if (Base::input::Keyboard::isClicked(BASE_KEY_H))
	{
		HideCursor();
		auto& scp = m_Camera.GetComponent<Base::NativeScriptComponent>();
		((Base::PerspectiveScript*)scp.Instance)->mouse_is_hide = true;
	}
	if (Base::input::Keyboard::isClicked(BASE_KEY_U))
	{
		UnhideCursor();
		auto& scp = m_Camera.GetComponent<Base::NativeScriptComponent>();
		((Base::PerspectiveScript*)scp.Instance)->mouse_is_hide = false;
	}

	auto& map_texture = m_Map.GetComponent<Base::TextureComponent>().Texture;
	for (auto& pair : m_Cubes)
	{
		auto& cube_model = pair.second.GetComponent<Base::ModelComponent>();
		cube_model.Model3D->OverrideTexture(map_texture);
	}

#endif	
	m_Scene->OnUpdate(args);
}

void Game::OnImGui()
{
	ImGui::Begin("Game Grid");

	static int grid = 24, a1 = 2, a2 = 3, rv = 3;

	ImGui::InputInt("Grid Size (width x height)", &grid);
	ImGui::InputInt("To be Alive 1", &a1);
	ImGui::InputInt("To be Alive 2", &a2);
	ImGui::InputInt("To Revive", &rv);
	if (ImGui::Button("Reset"))
	{
		m_Scene->DestroyNativeScript(m_Map);

		m_Scene->StartNativeScript(m_Map);
		auto& s = m_Map.GetComponent<Base::NativeScriptComponent>();
		((MapScript*)s.Instance)->p_Columns = grid;
		((MapScript*)s.Instance)->p_Rows = grid;
		((MapScript*)s.Instance)->ToBeAlive1 = a1;
		((MapScript*)s.Instance)->ToBeAlive2 = a2;
		((MapScript*)s.Instance)->ToRevive = rv;
		m_Scene->AwakeNativeScript(m_Map);
	}

#ifndef BASE_USING_3D
	auto& q = m_Map.GetComponent<Base::TransformComponent>();
	ImGui::SliderFloat3("Map Translation", &q.Translation.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Map Scale", &q.Scale.x, 0.0f, 20.0f);
	ImGui::SliderFloat3("Map Rotation", &q.Rotation.x, -10.0f, 10.0f);
#endif

	static char text[50] = "Map.png";
	ImGui::InputText("Path", text, 50);
	if (ImGui::Button("Save File"))
	{
		auto& Texture = m_Map.GetComponent<Base::TextureComponent>().Texture;
		Texture->CreatePng(text);
	}

	ImGui::End();

#ifdef BASE_USING_3D
	ImGui::Begin("Camera");

	auto& camera_script = m_Camera.GetComponent<Base::NativeScriptComponent>();
	float* vel = &((Base::PerspectiveScript*)camera_script.Instance)->default_speed;
	float* se = &((Base::PerspectiveScript*)camera_script.Instance)->default_sensitivity;

	if(ImGui::SliderFloat("Camera Velocity", vel , 1.0f, 10.0f))
		((Base::PerspectiveScript*)camera_script.Instance)->SyncSpeed();
	if (ImGui::SliderFloat("Camera Mouse S", se, 0.0001f, 1.0f))
		((Base::PerspectiveScript*)camera_script.Instance)->SyncSpeed();
	
	auto& cam = m_Camera.GetComponent<Base::CameraComponent>().Camera;

	ImGui::End();
#endif
	ImGui::Begin("Framebuffer");

	if (ImGui::SliderFloat("Scale Factor", &scale_factor, 0.05f, 5.0f))
	{
		int w = Base::WindowProps().width;
		int h = Base::WindowProps().height;
		m_Scene->SetFrameBuff(w, h, scale_factor);
}

	ImGui::Text("Post effects");
	const auto& effects = m_Scene->GetPostEffects();
	for (auto& efx : effects)
	{
		if (ImGui::Button(efx.first.c_str()))
			m_Scene->SetPostEffect(efx.first);
	}
	ImGui::End();
}

void Game::Dispose()
{
	m_Scene->SceneEnd();
}

void Game::OnResize(const Base::ResizeArgs& args)
{
	if (Base::WindowProps().minimized)
		return;
	auto& c = m_Camera.GetComponent<Base::CameraComponent>();
	c.Camera.SetViewportSize(args.new_w, args.new_h);
	m_Scene->SetFrameBuff(args.new_w, args.new_h,scale_factor);
}
