#include "Game.h"
#include <corecrt_math.h>
#include "scene/Components.h"
#include "scene/CameraScript.h"
#include "imgui.h"

#include "utils/RandomList.h"

Game::Game()
	:Base::windowing::Window()
{
}

Game::~Game()
{
}

void Game::OnAttach()
{

	m_Scene = Base::MakeScope <Base::Scene>();
	m_Map = m_Scene->CreateEntity("Map");
	m_Camera = m_Scene->CreateEntity("Main_Camera3D");

	auto& scp3D = m_Camera.AddComponent<Base::NativeScriptComponent>();
	{// 3D Camera
		scp3D.Bind<Base::PerspectiveScript>();
		auto& c = m_Camera.AddComponent<Base::CameraComponent>();
		auto& camera_transform = m_Camera.GetComponent<Base::TransformComponent>();

		c.Camera.SetPerspective(45.0f, 0.1f, 1000.0f);
		c.Camera.SetViewportSize(Base::WindowProps().width, Base::WindowProps().height);
	}

	m_Map.AddComponent<Base::TextureComponent>();
	auto& map_script = m_Map.AddComponent<Base::NativeScriptComponent>();
	m_Map.GetComponent<Base::TransformComponent>().Translation = { 1000.f,1000.0f,1000.0f };
	m_Map.GetComponent<Base::TransformComponent>().Scale = { 0,00.0f,000.0f };
	map_script.Bind<MapScript>();

	m_Scene->StartNativeScript(m_Map);
	m_Scene->AwakeNativeScript(m_Map);

	auto& map_texture = m_Map.GetComponent<Base::TextureComponent>().Texture;

	const int cube_qtd = 25;
	const float cube_spread = 1.5f;
	const glm::vec3 cube_size_variation = glm::vec3(1.0f, 1.0f, 1.0f);
	auto cube_3dmodel = Base::Model::CreateModel("resources/models/cube/cube.obj");
	for (int i = 0; i < cube_qtd; i++)
	{
		const std::string tag_name = "Cube_" + std::to_string(i);
		m_Cubes[tag_name] = m_Scene->CreateEntity(tag_name);

		auto& cube_transform = m_Cubes[tag_name].GetComponent<Base::TransformComponent>();
		//Cube position
		cube_transform.Translation =
		{
			(float)(M_random() << 2) * cube_spread,
			(float)(M_random() << 2) * cube_spread,
			(float)(M_random() << 2) * cube_spread
		};

		cube_transform.Rotation =
		{
			glm::radians((float)(M_random() << 1)),
			glm::radians((float)(M_random() << 1)),
			glm::radians((float)(M_random() << 1))
		};

		cube_transform.Scale =
		{
			(float)M_random() * cube_size_variation.x,
			(float)M_random() * cube_size_variation.y,
			(float)M_random() * cube_size_variation.z
		};

		auto& cube_model = m_Cubes[tag_name].AddComponent<Base::ModelComponent>(cube_3dmodel);
		cube_model.Model3D->OverrideTexture(map_texture);
	}

	m_Cubes["Cube_1"].GetComponent<Base::TransformComponent>().Translation = { 0.0f,0.0f,3.0f };

	m_Scene->StartNativeScript(m_Camera);

	HideCursor();
	((Base::PerspectiveScript*)scp3D.Instance)->mouse_is_hide = true;

}

void Game::OnUpdate(const Base::UpdateArgs& args)
{
	if (Base::WindowProps().minimized)
		return;

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

	m_Scene->OnUpdate(args);
	
}

void Game::OnImGui()
{
	ImGui::Begin("Game Grid");

	auto& q = m_Map.GetComponent<Base::TransformComponent>();

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
	
	static char text[50] = "Map.png";
	ImGui::InputText("Path", text, 50);
	if (ImGui::Button("Save File"))
	{
		auto& Texture = m_Map.GetComponent<Base::TextureComponent>().Texture;
		Texture->CreatePng(text);
	}
	ImGui::End();

	ImGui::Begin("Camera");

	auto& camera_script = m_Camera.GetComponent<Base::NativeScriptComponent>();
	float* vel = &((Base::PerspectiveScript*)camera_script.Instance)->default_speed;

	if(ImGui::SliderFloat("Camera Velocity", vel , 1.0f, 400.0f))
		((Base::PerspectiveScript*)camera_script.Instance)->SyncSpeed();

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
	auto& c3D = m_Camera.GetComponent<Base::CameraComponent>();
	c3D.Camera.SetViewportSize(args.new_w, args.new_h);
}
