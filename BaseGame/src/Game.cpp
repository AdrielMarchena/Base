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
	//m_TestQuad = m_Scene->CreateEntity("TestQuad");
	//m_TestQuad.AddComponent<Base::SpriteComponent>();

	auto& scp = m_Camera.AddComponent<Base::NativeScriptComponent>();
	scp.Bind<CameraScript>();
	auto& c = m_Camera.AddComponent<Base::CameraComponent>();
	c.Camera.SetViewportSize(Base::WindowProps().width, Base::WindowProps().height);
	
	auto& map_script = m_Map.AddComponent<Base::NativeScriptComponent>();
	map_script.Bind<MapScript>();
	m_Scene->StartNativeScript(m_Map);
	m_Scene->StartNativeScript(m_Camera);

}

void Game::OnUpdate(const Base::UpdateArgs& args)
{
	if (!Base::WindowProps().minimized)
		m_Scene->OnUpdate(args);
}

void Game::OnImGui()
{
	//ImGui::Begin("Test Quad");
	//
	//auto& q = m_TestQuad.GetComponent<Base::TransformComponent>();
	//ImGui::SliderFloat3("Translation",&q.Translation.x,-50.0f,50.0f);
	//ImGui::SliderFloat3("Scale", &q.Scale.x, -50.0f, 50.0f);
	//ImGui::SliderFloat3("Rotation", &q.Rotation.x, -50.0f, 50.0f);
	//
	//ImGui::End();
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
