#include "PiriquitoAbano.h"

#include "glm/gtx/compatibility.hpp"
#include "Log.h"

PiriquitoAbano::PiriquitoAbano()
	:en::windowing::Window("Piriquito Abano"),
	 m_Piriquito(),
	 m_Piper(),
	m_TextFont()
{
}

void PiriquitoAbano::OnAttach(en::AttachArgs args)
{
	LoadTextures("imgs");
	LoadText("fonts");
	LoadSounds("audio");

	args.camera.SetScroll(false);

	m_Lights.UpdateAmbient(en::windowing::Ambient::Middle * 1.5f, args.render.GetQuadShader(), false);
	m_Lights.UpdateAmbient(en::windowing::Ambient::Middle * 1.5f, args.render.GetCircleShader(), false);
	m_Lights.UpdateAmbient(en::windowing::Ambient::Middle * 1.5f, args.render.GetTriShader(), false);
	m_Lights.UpdateAmbient(en::windowing::Ambient::Middle * 1.5f, args.render.GetLineShader(), false);

	en::windowing::LightSource source;
	source.m_LightIntencity = 1500.0f;
	source.u_LightColor = { 0.9f,0.8f,0.2f,1.0f };
	source.u_LightPos.x = glm::lerp(0.0f, args.res_h, 0.99f);
	source.u_LightPos.y = glm::lerp(0.0f, args.res_w, 0.50f);

	m_Lights.AddStaticLightSource(source);

	m_Lights.UpdateStaticLight(args.render.GetQuadShader());
	m_Lights.UpdateStaticLight(args.render.GetCircleShader());
	m_Lights.UpdateStaticLight(args.render.GetLineShader());
	m_Lights.UpdateStaticLight(args.render.GetTriShader());

	m_Piriquito.SetTexture(m_Textures["priquito_atlas"]);
	m_Piriquito.OnAttach(args);
	m_Piriquito.SetFeatherTexture(m_Textures["pena"]);

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
	StartBox.out_text = "Press Space To start";
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

	//HideCursor();
	Window::m_ClampMouse = false;

	m_Back.OnAttach(args);

	//m_Audios["alexander-nakarada-the-great-battle"].Loop(true);

	Window::OnAttach(args);
	SetResizeble(false);
	//m_Audios["alexander-nakarada-the-great-battle"].Play();
}

void PiriquitoAbano::SwitchPause()
{
	if (m_State == PiriquitoState::PLAYING)
	{
		m_State = PiriquitoState::PAUSE;
		//UnhideCursor();
		return;
	}
	if (m_State == PiriquitoState::PAUSE)
	{
		m_State = PiriquitoState::PLAYING;
		//HideCursor();
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

	if (args.keyboard.isClicked(GLFW_KEY_R))
		Restart(args);
	if (args.keyboard.isClicked(GLFW_KEY_P))
		SwitchPause();
	//Update Piriquito
	if (m_State == PiriquitoState::PLAYING)
	{
		m_Back.OnUpdate(args);
		m_Piriquito.OnUpdate(args, { m_Gravity });
		m_Piper.SpawnPairPipe(args.dt);
		m_Piper.UpdatePipes(args);
		for (auto& pipe : m_Piper.GetPipes())
		{
			if (!pipe.IsAlive())
				continue;
 			if (en::colision::Colide::RectVsRect(m_Piriquito.GetColision(), pipe.GetRect()))
			{
				if (pipe.pointPipe)
				{
					pipe.Despawn();
					points++;
					m_Piper.pipe_velocity = m_Piper.pipe_velocity + (points * 0.15f);
					m_Piper.threshold = m_Piper.threshold - (points * 0.10f);
					m_Piper.PipeGap = m_Piper.PipeGap - (points * 0.001f);
					m_Piriquito.SetAnimVel(m_Piriquito.GetAnimVel() + (points * 0.15f));
					m_Back.m_Specs.b_velocity += (points * 0.15f);
					m_Back.m_Specs.m_velocity += (points * 0.15f);
					m_Back.m_Specs.f_velocity += (points * 0.15f);
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
		if (m_State == PiriquitoState::GAMEOVER){
			if (args.keyboard.isClicked(GLFW_KEY_SPACE))
			{
				Restart(args);
				m_Piper.Reset();
			}else
				GameOverBox.OnUpdate(args);
		}
		if (m_State == PiriquitoState::PAUSE)
		{
			if (args.keyboard.isClicked(GLFW_KEY_SPACE))
			{
				SwitchPause();
			}
			else
				StartBox.OnUpdate(args);
		}
	}

	Window::OnUpdate(args);
}

void PiriquitoAbano::OnRender(en::RenderArgs args)
{
	m_Back.OnRender(args);
	//Draw Piriquito
	if (m_State == PiriquitoState::GAMEOVER)
		GameOverBox.OnRender(args, m_TextFont);
	if (m_State == PiriquitoState::PAUSE)
		StartBox.OnRender(args, m_TextFont);
	
	m_Piriquito.OnRender(args);
	m_Piper.RenderPipes(args);
	const std::string pts = "Points:" + std::to_string(points);
	m_TextFont->RenderText(args, pts, 25.0f, 550.0f, 1.0f, {0.1f,0.1f,0.1f});

	static const glm::vec4 pointer_colors[3]
	{
		{1.0f,0.0f,0.0f,0.9f},
		{0.0f,1.0f,0.0f,0.9f},
		{0.0f,0.0f,1.0f,0.9f}
	};

	args.render.DrawTriangle(m_Pointer, pointer_colors, 10.0f);
	Window::OnRender(args);
}

void PiriquitoAbano::OnImGui(en::ImGuiArgs args)
{
	//m_Piriquito.OnImGui(args);
	//m_Piper.OnImGui(args);

	Window::OnImGui(args);
}

void PiriquitoAbano::Dispose()
{
	/*for (auto& audio : m_Audios)
		audio.second.Dispose();*/
	m_Textures.Map([&](std::shared_ptr<en::render::Texture> texture_sptr) {
		texture_sptr->Dispose();
	});
	Window::Dispose();
}

void PiriquitoAbano::OnMouseAction(en::MouseArgs args)
{
	Window::OnMouseAction(args);

	switch (args.m_action)
	{
	case en::MouseAction::ENTER :
		//HideCursor();
		break;
	case en::MouseAction::LEAVE:
		UnhideCursor();
		break;
	default:
		m_Pointer[0] = m_pos(args.mouse);
		m_Pointer[1] = m_Pointer[0] - (glm::vec2(-15.0f, 7.5f) * 1.50f);
		m_Pointer[2] = m_Pointer[0] - (glm::vec2(-7.5f, 15.0f) * 1.50f);
		break;
	}
}

void PiriquitoAbano::Restart(en::UpdateArgs args)
{
		m_Piriquito.GetRect().pos.y = 300.0f;
		m_Piper.Reset();
		m_Back.m_Specs = Back_Spec();
		points = 0;
		m_State = PiriquitoState::PAUSE;
}

void PiriquitoAbano::LoadTextures(const char* directory)
{
	using namespace en;
	//Texuture
	try
	{
		m_Textures = render::Texture::LoadAsyncTexture(utils::Files::GetPairText(directory));
	}
	catch (const utils::baseException::directory_not_found& dex)
	{
		//TODO: Put a default texture on the u_map (do not rethrow)
		APP_ERROR("Directory to Textures ({0}) not found!", dex.path());
	}
	catch (const std::exception& ex)
	{
		//TODO: Put a default texture on the u_map or maybe throw this again
		APP_ERROR("Can't create Texture(s), Error: {0}", ex.what());
		//LOG_NORMAL("Can't create Texture(s), Error: " << ex.what());
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
		APP_ERROR("Directory to Audios ({0}) not found!", dex.path());
	}
	catch (const std::exception& ex)
	{
		//TODO: Put a default audio on the u_map or maybe throw this again
		APP_ERROR("Can't create Audio(s), Error: {0}", ex.what());
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
		APP_ERROR("Directory to Fonts ({0}) not found!", dex.path());
	}
	catch (const std::exception& ex)
	{
		//TODO: Put a default audio on the u_map or maybe throw this again
		APP_ERROR("Can't create Font(s), Error: {0}", ex.what());
	}
}