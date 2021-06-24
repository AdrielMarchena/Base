#include "Game.h"
#include <corecrt_math.h> //float_t is typedef here


static inline void MainMenuBar(const ImGuiArgs& args)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			bool exit_opt = ImGui::MenuItem("Exit", "Ctrl+Q", false);

			if (exit_opt)
				exit(0);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

Game::Game(const char* title, float_t w, float_t h, bool resizeble)
	:Window(title, w, h, resizeble),CellGame(Map()), rect_a()
{
}

void Game::OnAttach(AttachArgs args)
{
	//LoadTextures("test_imgs");
	//LoadSounds("test_audio");

	std::vector<InitActiveCell> actives;
	//actives.push_back({0,0});
	actives.push_back({8,8});
	actives.push_back({9,8});
	actives.push_back({10,8});
	//actives.push_back({11,8});

	//CellGame.OnAttach(actives);

	
	m_Ambient.AddStaticLightSource({
		{50.0f,50.0f,0.0f},
		{1.0f,1.0f,1.0f,1.0f},
		1.0f
		});

	m_Ambient.AddStaticLightSource({
		{550.0f,550.0f,0.0f},
		{1.0f,1.0f,1.0f,1.0f},
		1.0f
		});

	m_Ambient.UpdateStaticLight(args.render.GetQuadShader());
	m_Ambient.UpdateAmbient(windowing::Ambient::Day, args.render.GetQuadShader());
	
	m_Ambient.UpdateStaticLight(args.render.GetLineShader());
	m_Ambient.UpdateAmbient(windowing::Ambient::Day, args.render.GetLineShader());

	m_Ambient.UpdateStaticLight(args.render.GetCircleShader());
	m_Ambient.UpdateAmbient(windowing::Ambient::Day, args.render.GetCircleShader());

	//m_Ambient.ZeroLight(args.render.GetQuadShader());
	//m_Ambient.ZeroLight(args.render.GetLineShader());
	//m_Ambient.ZeroLight(args.render.GetCircleShader());

	rect_a =
	{
		{400,400},
		{50,50},
		{0,0}
	};

	Window::OnAttach(args);
}

void Game::OnUpdate(UpdateArgs args)
{
	//CellGame.UpdateCells(args);

	const glm::vec2 ray_origin = { 0,0 };
	glm::vec2 cp = { 0,0 };
	glm::vec2 cn = { 0,0 };
	float hit = 0;
		m_RenderThisPlease.push_back([&](RenderArgs& r_args)
		{
			r_args.render.DrawLine(ray_origin, args.m_pos, { 0.2f,0.7f,0.9f,1.0f });
			if (colision::Colide::RayVsRect(ray_origin, args.m_pos, rect_a, cp, cn, hit) && hit <= 1.0f)
			{
				r_args.render.DrawCircle(cp, 50 , { 1.0f,0.0f,0.0f,1.0f }, false);
				r_args.render.DrawLine(cp, cp + (cn * 5.0f) , { 1.0f,0.0f,0.0f,1.0f });
			}
			if (args.mouse.isPress(GLFW_MOUSE_BUTTON_1))
			{
				r_args.render.DrawCircle(args.m_pos, 50, { 0.0f,0.0f,1.0f,1.0f });
				r_args.render.DrawCircle(args.m_pos, 75, { 1.0f,0.0f,0.0f,1.0f }, false);
			}
		});

	Window::OnUpdate(args);
}

void Game::OnRender(RenderArgs args)
{
	//CellGame.DrawCells(args);

	//args.render.DrawOutLineQuad({ 0,0 }, { 100,100 }, {1.0f,1.0f,1.0f,1.0f});
	//args.render.DrawLine({ 0,0 }, { -100,-100 }, { 1.0f,1.0f,1.0f,1.0f });
	//args.render.DrawCircle({ 100,100 }, 500.0f, { 1.0f,1.0f,1.0f,1.0f });
	//args.render.DrawCircle({ 100,100 }, 100.0f, { 0.0f,0.0f,1.0f,1.0f },1);

	args.render.DrawQuad(rect_a.pos, rect_a.size, { 1.0f,0.0f,0.0f,1.0f });

	for (auto& lamb : m_RenderThisPlease)
		lamb(args);
	m_RenderThisPlease.clear();
	
	Window::OnRender(args);
}

void Game::OnImGui(ImGuiArgs args)
{
	MainMenuBar(args);

	//CellGame.OnImGui(args);

	ImGui::SliderFloat("Ambient Light", &m_ImInfo.LightAmbient.x, 0.0f, 1.0f);
	ImGui::SliderFloat("Light source Force", &m_ImInfo.LightForce, 0.0f, 1000.0f);
	ImGui::SliderFloat("Light source Force 2", &m_ImInfo.LightForce2, 0.0f, 1000.0f);

	m_ImInfo.LightAmbient = { m_ImInfo.LightAmbient.x ,m_ImInfo.LightAmbient.x ,m_ImInfo.LightAmbient.x };

	m_Ambient[0].m_LightIntencity = m_ImInfo.LightForce;
	m_Ambient[1].m_LightIntencity = m_ImInfo.LightForce2;

	m_Ambient.UpdateStaticLight(args.render.GetCircleShader());
	m_Ambient.UpdateAmbient(m_ImInfo.LightAmbient, args.render.GetCircleShader());

	m_Ambient.UpdateStaticLight(args.render.GetLineShader());
	m_Ambient.UpdateAmbient(m_ImInfo.LightAmbient, args.render.GetLineShader());

	m_Ambient.UpdateStaticLight(args.render.GetQuadShader());
	m_Ambient.UpdateAmbient(m_ImInfo.LightAmbient, args.render.GetQuadShader());

	Window::OnImGui(args);
}

void Game::Dispose()
{
	m_Textures.clear();

	Window::Dispose();
}

void Game::LoadTextures(const char* directory)
{
	//Texuture
	try
	{
		m_Textures = render::Texture::LoadAsyncTextures(utils::Files::GetPairText(directory));
	}
	catch (const utils::baseException::directory_not_found& dex)
	{
		//TODO: Put a default texture on the u_map (do not rethrow)
		LOG_NORMAL("Directory to Textures " << dex.path() << " not found!");
	}
	catch (const std::exception& ex)
	{
		//TODO: Put a default texture on the u_map or maybe throw this again
		LOG_NORMAL("Can't create Textures, Error: " << ex.what());
	}
}

void Game::LoadSounds(const char* directory)
{
	//Audios
	try
	{
		m_Audios = aux::AudioSource::LoadAudios(utils::Files::GetPairText(directory, ".mp3#.ogg#.wav"));
	}
	catch (const utils::baseException::directory_not_found& dex)
	{
		//TODO: Put a default texture on the u_map (do not rethrow please)
		LOG_NORMAL("Directory to Audios " << dex.path() << " not found!");
	}
	catch (const std::exception& ex)
	{
		//TODO: Put a default audio on the u_map or maybe throw this again
		LOG_NORMAL("Can't create Audios, Error: " << ex.what());
	}
}
