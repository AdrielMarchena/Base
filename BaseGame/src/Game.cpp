#include "Game.h"
#include <corecrt_math.h>
#include "scene/Components.h"
#include "scene/CameraScript.h"
#include "imgui.h"

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
	m_Camera = m_Scene->CreateEntity("Main_Camera");

	auto& scp = m_Camera.AddComponent<Base::NativeScriptComponent>();
	scp.Bind<Base::OrthoCameraScript>();
	auto& c = m_Camera.AddComponent<Base::CameraComponent>();
	auto& camera_transform = m_Camera.GetComponent<Base::TransformComponent>();

	camera_transform.Translation.z = 0.0f;

	c.Camera.SetPerspective(45, 0.1f, 100.0f);
	c.Camera.SetViewportSize(Base::WindowProps().width, Base::WindowProps().height);

	m_Map.AddComponent<Base::TextureComponent>();
	auto& map_script = m_Map.AddComponent<Base::NativeScriptComponent>();
	map_script.Bind<MapScript>();
	m_Scene->StartNativeScript(m_Map);
	m_Scene->AwakeNativeScript(m_Map);
	m_Scene->StartNativeScript(m_Camera);

}

void Game::OnUpdate(const Base::UpdateArgs& args)
{
	if (!Base::WindowProps().minimized)
		m_Scene->OnUpdate(args);
}

void Game::OnImGui()
{
	ImGui::Begin("Game Grid");
	//
	auto& q = m_Map.GetComponent<Base::TransformComponent>();
	if (ImGui::SliderFloat("Scale", &q.Scale.x, 1.0f, 150.0f))
	{
		q.Scale.y = q.Scale.x;
	}

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

	//
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
}
