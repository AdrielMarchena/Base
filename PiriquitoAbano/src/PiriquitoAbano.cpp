#include "PiriquitoAbano.h"

//Blank Texture
en::render::Texture BlankTexture;

PiriquitoAbano::PiriquitoAbano()
	:en::windowing::Window("Piriquito Abano"),
	 m_Piriquito(BlankTexture)
{
}

void PiriquitoAbano::OnAttach(en::AttachArgs args)
{
	LoadTextures("imgs");
	LoadText("fonts");
	LoadSounds("audio");

	m_Piriquito.SetTexture(m_Textures["priquito"]);
	m_Piriquito.OnAttach(args);

	m_Piper.OnAttach(args);

	m_Piper.SetPipesTexture(m_Textures["pipe"]);

	m_State = PiriquitoState::PAUSE;
	Window::OnAttach(args);
}

static inline void SwitchPause(PiriquitoState& state)
{
	if (state == PiriquitoState::PLAYING)
	{
		state = PiriquitoState::PAUSE;
		return;
	}
	if (state == PiriquitoState::PAUSE)
	{
		state = PiriquitoState::PLAYING;
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
	if (args.keyboard.isClicked(GLFW_KEY_P))
		SwitchPause(m_State);
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
		{
			Restart(args);
		}
	}

	Window::OnUpdate(args);
}

void PiriquitoAbano::OnRender(en::RenderArgs args)
{
	//Draw Piriquito
	args.render.DrawQuad({ 0,0 }, { 800,600 }, m_Textures["back"], 0.0f);
	m_Piriquito.OnRender(args);
	m_Piper.RenderPipes(args);
	const std::string pts = "Points: " + std::to_string(points);
	m_Text["arial"].RenderText(args, pts, 25.0f, 550.0f, 1.0f, {0.1f,0.1f,0.1f});

	Window::OnRender(args);
}

void PiriquitoAbano::OnImGui(en::ImGuiArgs args)
{
	
	m_Piriquito.OnImGui(args);

	Window::OnImGui(args);
}

void PiriquitoAbano::Dispose()
{

	Window::Dispose();
}

void PiriquitoAbano::Restart(en::UpdateArgs args)
{
	//if (args.keyboard.isClicked(GLFW_KEY_R))
	//{
		m_Piriquito.GetRect().pos.y = 300.0f;
		for (auto& pipe : m_Piper.GetPipes())
		{
			pipe.Despawn();
			points = 0;
		}
		m_State = PiriquitoState::PAUSE;
	//}
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