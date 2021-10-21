#include "Game.h"
#include <corecrt_math.h>
#include "scene/Components.h"
#include "scene/CameraScript.h"
#include "imgui.h"
#include "utils/Files.h"

#include "utils/Load.h"

Base::utils::Loader<Base::render::ImageInformation>* loader = nullptr;


Game::Game()
	:Base::windowing::Window()
{
}

Game::~Game()
{
}

void Game::OnAttach()
{
	loader = new Base::utils::Loader<Base::render::ImageInformation>("resources/images");

	//LoadAsync();

	auto dirs = Base::utils::Files::GetPairText("resources/images");

	for (auto& d : dirs)
	{
		auto info = Base::render::Texture::GetImageInfo(d.second);
		info.KeepSourceBuffer = false;
		info.DeleteSourceBuffer = true;
		m_Textures.emplace(d.first, Base::MakeRef<Base::render::Texture>(info));
	}

	m_Scene = Base::MakeScope <Base::Scene>();
	m_Map = m_Scene->CreateEntity("Map");
	m_Camera = m_Scene->CreateEntity("Main_Camera3D");
	m_Camera2D = m_Scene->CreateEntity("Main_Camera2D");
	m_Sprite = m_Scene->CreateEntity("Sprite");

	auto& scp3D = m_Camera.AddComponent<Base::NativeScriptComponent>();
	{// 3D Camera
		scp3D.Bind<Base::PerspectiveScript>();
		auto& c = m_Camera.AddComponent<Base::CameraComponent>();
		auto& camera_transform = m_Camera.GetComponent<Base::TransformComponent>();

		c.Camera.SetPerspective(45.0f, 0.1f, 1000.0f);
		c.Camera.SetViewportSize(Base::WindowProps().width, Base::WindowProps().height);
	}
	
	{// 2D Camera
		//auto& scp2D = m_Camera2D.AddComponent<Base::NativeScriptComponent>();
		//scp2D.Bind<Base::OrthoCameraScript>();
		auto& c = m_Camera2D.AddComponent<Base::CameraComponent>();
		auto& camera_transform = m_Camera2D.GetComponent<Base::TransformComponent>();

		c.Camera.SetViewportSize(Base::WindowProps().width, Base::WindowProps().height);
	}

	m_Sprite.AddComponent<Base::TextureComponent>();

	m_Cube = m_Scene->CreateEntity("Cube");
	
	auto& cube_transform = m_Cube.GetComponent<Base::TransformComponent>();
	cube_transform.Translation.z = 3.0f;

	auto& cube_model = m_Cube.AddComponent<Base::ModelComponent>(Base::Model::CreateModel("resources/models/cube/cube.obj"));

	m_Map.AddComponent<Base::TextureComponent>();
	auto& map_script = m_Map.AddComponent<Base::NativeScriptComponent>();
	m_Map.GetComponent<Base::TransformComponent>().Translation = { 1000.f,1000.0f,1000.0f };
	m_Map.GetComponent<Base::TransformComponent>().Scale = { 0,00.0f,000.0f };
	map_script.Bind<MapScript>();

	m_Scene->StartNativeScript(m_Map);
	m_Scene->AwakeNativeScript(m_Map);

	auto& map_texture = m_Map.GetComponent<Base::TextureComponent>().Texture;
	cube_model.Model3D->OverrideTexture(map_texture);

	cube_transform.Scale = { 100.0f,100.0f,100.0f };

	m_Scene->StartNativeScript(m_Camera);

	m_Sprite.GetComponent<Base::TransformComponent>().Scale = { 1.0f,1.0f,1.0f };


	HideCursor();
	((Base::PerspectiveScript*)scp3D.Instance)->mouse_is_hide = true;

	//while(!loader->DoAsyncOne<Base::render::Texture>(m_Textures)){}
	//delete loader;
	//loader = nullptr;
}

void Game::OnUpdate(const Base::UpdateArgs& args)
{
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

	if (Base::WindowProps().minimized)
		return;
	m_Scene->OnUpdate(args);

	if(loader)
		if (loader->DoAsyncOne<Base::render::Texture>(m_Textures))
		{
			delete loader;
			loader = nullptr;
		}

	auto& map_texture = m_Map.GetComponent<Base::TextureComponent>().Texture;
	auto& cube_model = m_Cube.GetComponent<Base::ModelComponent>();
	cube_model.Model3D->OverrideTexture(map_texture);
	m_Map.GetComponent<Base::TransformComponent>().Scale = { 0,00.0f,000.0f };

}

void Game::OnImGui()
{
	ImGui::Begin("Game Grid");
	//
	auto& q = m_Map.GetComponent<Base::TransformComponent>();
	auto& cube_transform = m_Cube.GetComponent<Base::TransformComponent>();
	if (ImGui::SliderFloat("Scale", &cube_transform.Scale.x, 1.0f, 1500.0f))
	{
		cube_transform.Scale.y = cube_transform.Scale.x;
		cube_transform.Scale.z = cube_transform.Scale.x;
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

	for (auto& t : m_Textures)
	{
		if (ImGui::Button(t.first.c_str()))
		{
			auto& tex = m_Sprite.GetComponent<Base::TextureComponent>();
			tex.Texture = t.second;
		}
	}
	//
	ImGui::End();
}

void Game::Dispose()
{
	m_Scene->SceneEnd();
	if(loader)
		delete loader;
}

void Game::OnResize(const Base::ResizeArgs& args)
{
	if (Base::WindowProps().minimized)
		return;
	auto& c3D = m_Camera.GetComponent<Base::CameraComponent>();
	c3D.Camera.SetViewportSize(args.new_w, args.new_h);
	auto &c2D = m_Camera2D.GetComponent<Base::CameraComponent>();
	c2D.Camera.SetViewportSize(args.new_w, args.new_h);
}

bool Game::LoadAsync()
{
	loader->LauchAsync([&](const std::string& name, const std::string& path)
		{
			Base::render::ImageInformation info = Base::render::Texture::GetImageInfo(path);
			info.Name = name;
			info.DeleteSourceBuffer = true;
			info.KeepSourceBuffer = false;
			info.CopySourceBuffer = false;
			//std::lock_guard<std::mutex> lock(VectorMutex);
			//ImageInfo.push_back(info);
			return info;
		});
	return true;
}