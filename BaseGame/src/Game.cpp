#include "Game.h"
#include <corecrt_math.h>
#include "scene/Components.h"

#include "imgui.h"

class CameraScript : public Base::ScriptableEntity
{
protected:
	virtual void OnUpdate(const Base::UpdateArgs& args) override
	{
		auto& T = GetComponent<Base::TransformComponent>().Translation;
		using kb = Base::input::Keyboard;

		constexpr float vel = 5.5f;

		if (kb::isPress(BASE_KEY_D))
			T.x += vel * args.dt;
		if (kb::isPress(BASE_KEY_A))
			T.x -= vel * args.dt;
		if (kb::isPress(BASE_KEY_W))
			T.y += vel * args.dt;
		if (kb::isPress(BASE_KEY_S))
			T.y -= vel * args.dt;
	}
};

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
	scp.Bind<CameraScript>();
	auto& c = m_Camera.AddComponent<Base::CameraComponent>();
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

	static int grid = 24;

	ImGui::InputInt("Grid Size (width x height)", &grid);

	if (ImGui::Button("Reset"))
	{
		m_Scene->DestroyNativeScript(m_Map);

		m_Scene->StartNativeScript(m_Map);
		auto& s = m_Map.GetComponent<Base::NativeScriptComponent>();
		((MapScript*)s.Instance)->p_Columns = grid;
		((MapScript*)s.Instance)->p_Rows = grid;
		m_Scene->AwakeNativeScript(m_Map);
	}

	static char text[50] = "Map.png";
	ImGui::InputText("Path", text, 50);
	if (ImGui::Button("Save File"))
	{
		auto& s = m_Map.GetComponent<Base::NativeScriptComponent>();
		Base::render::Texture::CreatePNGFile(text, grid, grid, ((MapScript*)s.Instance)->GetTexBuffer());
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
