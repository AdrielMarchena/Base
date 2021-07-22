#include "PiriquitoAbano.h"

//Blank Texture
static en::render::Text VeryDumbStupidTemporaryText;

PiriquitoAbano::PiriquitoAbano()
	:en::windowing::Window("Piriquito Abano"),
	 m_Piriquito(en::render::Texture::GetBlanckTexture()),
	m_TextFont(VeryDumbStupidTemporaryText)
{
}

void PiriquitoAbano::OnAttach(en::AttachArgs args)
{
	LoadTextures("imgs");
	LoadText("fonts");
	LoadSounds("audio");

	m_Piriquito.SetTexture(m_Textures["priquito_atlas"]);
	m_Piriquito.OnAttach(args);

	m_Piper.OnAttach(args);

	m_TextFont = m_Text["consolab"];

	//TODO: I need to improve the way i delivery the resources throw out the program
	m_Piper.SetPipesTexture(m_Textures["pipe"]);
	m_Piper.SetPipesBodyTexture(m_Textures["Pipe_body"]);

	m_State = PiriquitoState::PAUSE;
	
	GameOverBox.out_box = { {800 / 2,600 / 2},{300,200},{0,0} };
	GameOverBox.button = { {},{100,50},{} };
	GameOverBox.out_text = "Game Over";
	GameOverBox.button_text = "Try Again";
	GameOverBox.out_text_color = { 1.0f,1.0f,1.0f };
	GameOverBox.button_text_color = { 1.0f,1.0f,1.0f };
	GameOverBox.hover_button_text_color = { 1.0f,0.0f,0.0f };
	GameOverBox.button_action = [&](const en::UpdateArgs& args)
	{
		Restart(args);
	};

	StartBox.out_box = { {800 / 2,600 / 2},{300,200},{0,0} };
	StartBox.button = { {},{100,50},{} };
	StartBox.out_text = "Press P To start";
	StartBox.button_text = "Start";
	StartBox.out_text_color = { 1.0f,1.0f,1.0f };
	StartBox.button_text_color = { 1.0f,1.0f,1.0f };
	StartBox.hover_button_text_color = { 1.0f,0.0f,0.0f };
	StartBox.button_action = [&](const en::UpdateArgs& args)
	{
		SwitchPause();
	};
	
	GameOverBox.AdjustPositions(m_TextFont);
	StartBox.AdjustPositions(m_TextFont);

	HideCursor();

	Window::OnAttach(args);
}

void PiriquitoAbano::SwitchPause()
{
	if (m_State == PiriquitoState::PLAYING)
	{
		m_State = PiriquitoState::PAUSE;
		UnhideCursor();
		return;
	}
	if (m_State == PiriquitoState::PAUSE)
	{
		m_State = PiriquitoState::PLAYING;
		HideCursor();
		return;
	}
}

void PiriquitoAbano::OnUpdate(en::UpdateArgs args)
{
	static en::Rect Bondery =
	{
		{64.0f,32.0f},
		{800.0f,600.0f},
		{0,0}
	};

	m_Pointer[0].x = std::clamp(args.m_pos.x, 0.0f,800.0f);
	m_Pointer[0].y = std::clamp(args.m_pos.y, 0.0f, 600.0f);
	m_Pointer[1] = m_Pointer[0] - (glm::vec2(-15.0f, 7.5f) * 1.50f);
	m_Pointer[2] = m_Pointer[0] - (glm::vec2(-7.5f, 15.0f) * 1.50f);

	if (args.keyboard.isClicked(GLFW_KEY_P))
		SwitchPause();
	if (args.keyboard.isClicked(GLFW_KEY_R))
		Restart(args);
	//Update Piriquito
	if (m_State == PiriquitoState::PLAYING)
	{
		m_Piriquito.OnUpdate(args, { m_Gravity });
		m_Piper.SpawnPairPipe(args.dt);
		m_Piper.UpdatePipes(args);
		for (auto& pipe : m_Piper.GetPipes())
		{
			if (!pipe.IsAlive())
				continue;
			//tmpPipe.size.y = std::fabs(tmpPipe.size.y);
			if (en::colision::Colide::RectVsRect(m_Piriquito.GetRect(), pipe.GetRect()))
			{
				if (pipe.pointPipe)
				{
					pipe.Despawn();
					points++;
					break;
				}
				m_Piriquito.Die();
				m_State = PiriquitoState::GAMEOVER;
				break;
			}
		}
		if (!en::colision::Colide::RectVsRect(m_Piriquito.GetRect(), Bondery))
		{
			m_Piriquito.Die();
			m_State = PiriquitoState::GAMEOVER;
		}
	}
	else
	{
		if (m_State == PiriquitoState::GAMEOVER)
			GameOverBox.OnUpdate(args);
		if (m_State == PiriquitoState::PAUSE)
			StartBox.OnUpdate(args);
	}

	Window::OnUpdate(args);
}

void PiriquitoAbano::OnRender(en::RenderArgs args)
{
	args.render.DrawQuad({ 0,0 }, { 800,600 }, m_Textures["back"], 0.0f);
	//Draw Piriquito
	if (m_State == PiriquitoState::GAMEOVER)
		GameOverBox.OnRender(args, m_TextFont);
	if (m_State == PiriquitoState::PAUSE)
		StartBox.OnRender(args, m_TextFont);
	
	m_Piriquito.OnRender(args);
	m_Piper.RenderPipes(args);
	const std::string pts = "Points:" + std::to_string(points);
	m_TextFont.RenderText(args, pts, 25.0f, 550.0f, 1.0f, {0.1f,0.1f,0.1f});

	static const glm::vec4 colors[3]
	{
		{1.0f,0.0f,0.0f,0.9f},
		{0.0f,1.0f,0.0f,0.9f},
		{0.0f,0.0f,1.0f,0.9f}
	};

	args.render.DrawTriangle(m_Pointer, colors, 10.0f);

	Window::OnRender(args);
}

void PiriquitoAbano::OnImGui(en::ImGuiArgs args)
{
	m_Piriquito.OnImGui(args);
	m_Piper.OnImGui(args);

	Window::OnImGui(args);
}

void PiriquitoAbano::Dispose()
{

	Window::Dispose();
}

void PiriquitoAbano::OnMouseAction(en::MouseArgs args)
{
	Window::OnMouseAction(args);

	switch (args.m_action)
	{
	case en::MouseAction::ENTER :
		HideCursor();
		break;
	case en::MouseAction::LEAVE:
		UnhideCursor();
		break;
	default:
		break;
	}
}

void PiriquitoAbano::Restart(en::UpdateArgs args)
{
		m_Piriquito.GetRect().pos.y = 300.0f;
		for (auto& pipe : m_Piper.GetPipes())
		{
			pipe.Despawn();
			points = 0;
		}
		m_State = PiriquitoState::PAUSE;
}

void PiriquitoAbano::LoadTextures(const char* directory)
{
	using namespace en;
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

void PiriquitoAbano::LoadSounds(const char* directory)
{
	using namespace en;
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

void PiriquitoAbano::LoadText(const char* directory)
{
	using namespace en;
	//Audios
	try
	{
		m_Text = en::render::Text::LoadFontsAsync(utils::Files::GetPairText(directory, ".ttf#.TTF#"));
	}
	catch (const utils::baseException::directory_not_found& dex)
	{
		//TODO: Put a default texture on the u_map (do not rethrow please)
		LOG_NORMAL("Directory to Text " << dex.path() << " not found!");
	}
	catch (const std::exception& ex)
	{
		//TODO: Put a default audio on the u_map or maybe throw this again
		LOG_NORMAL("Can't create Fonts, Error: " << ex.what());
	}
}