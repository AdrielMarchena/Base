#include "Game.h"
#include <corecrt_math.h> //float_t is typedef here

#include <filesystem>

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

static inline void test_movement(Rect& move, Rect& stati_c,UpdateArgs args)
{
	auto& keyb = args.keyboard;
	static float_t term_velocity = 1950.0f;
	static float_t ac_velocity = 950.0f;
	static float_t stop_velocity = 350.0f;

	if (keyb.isPress(GLFW_KEY_W))
	{
		if (move.velocity.y < term_velocity)
			move.velocity.y += ac_velocity * args.dt;
	}
	if (keyb.isPress(GLFW_KEY_A))
	{
		if (move.velocity.x > -term_velocity)
			move.velocity.x -= ac_velocity * args.dt;
	}
	if (keyb.isPress(GLFW_KEY_S))
	{
		if (move.velocity.y > -term_velocity)
			move.velocity.y -= ac_velocity * args.dt;
	}
	if (keyb.isPress(GLFW_KEY_D))
	{
		if (move.velocity.x < term_velocity)
			move.velocity.x += ac_velocity * args.dt;
	}

	glm::vec2 cp, cn;
	float_t ct = 0;

	if(colision::Colide::DynamicRectVsRect(move, stati_c, cp, cn, ct, args.dt))
		move.velocity += cn * (glm::vec2(std::abs(move.velocity.x), std::abs(move.velocity.y)) * (1 - ct));

	if (move.velocity.x > 0.0f)
	{
		move.velocity.x -= stop_velocity * args.dt;
		if (move.velocity.x < 0)
			move.velocity.x = 0.0f;
	}
	if (move.velocity.x < 0.0f)
	{
		move.velocity.x += stop_velocity * args.dt;
		if (move.velocity.x > 0)
			move.velocity.x = 0.0f;
	}

	if (move.velocity.y > 0.0f)
	{
		move.velocity.y -= stop_velocity * args.dt;
		if (move.velocity.y < 0)
			move.velocity.y = 0.0f;
	}
	if (move.velocity.y < 0.0f)
	{
		move.velocity.y += stop_velocity * args.dt;
		if (move.velocity.y > 0)
			move.velocity.y = 0.0f;
	}

	if (move.velocity.x > term_velocity)
		move.velocity.x = term_velocity;
	if (move.velocity.x < -term_velocity)
		move.velocity.x = -term_velocity;

	if (move.velocity.y > term_velocity)
		move.velocity.y = term_velocity;
	if (move.velocity.y < -term_velocity)
		move.velocity.y = -term_velocity;

}

Game::Game(const char* title, float_t w, float_t h, bool resizeble)
	:Window(title, w, h, resizeble),m_emiter(100)
{
}

void Game::OnAttach(AttachArgs args)
{
	//Texuture
	try
	{
		m_Textures = render::Texture::LoadAsyncTextures(utils::Files::GetPairText("test_imgs"));
	}
	catch (const utils::ex::directory_not_found& dex)
	{
		//TODO: Put a default texture on the u_map (do not rethrow)
		LOG_NORMAL("Directory to Textures " << dex.path() << " not found!");
	}
	catch (const std::exception& ex)
	{
		//TODO: Put a default texture on the u_map or maybe throw this again
		LOG_NORMAL("Can't create Textures, Error: " << ex.what());
	}
	//Audios
	try
	{
		m_Audios = aux::AudioSource::LoadAudios(utils::Files::GetPairText("test_audio", ".mp3#.ogg#.wav"));
	}
	catch (const utils::ex::directory_not_found& dex)
	{
		//TODO: Put a default texture on the u_map (do not rethrow please)
		LOG_NORMAL("Directory to Audios " << dex.path() << " not found!");
	}
	catch (const std::exception& ex)
	{
		//TODO: Put a default audio on the u_map or maybe throw this again
		LOG_NORMAL("Can't create Audios, Error: " << ex.what());
	}
	//Quads
	test_rect1 =
	{
		{50,50},
		glm::vec2(50,50) * 5.0f,
		{0,0}
	};
	test_rect2 =
	{
		{350,350},
		{50,50},
		{0,0}
	};

	ett::AnimationSpecs specs(m_Textures["atlas"]);
	specs.decrement = 10.0f;
	
	m_animator.SetNewTexture(specs);
	m_animator.loop = true;

	m_ambient.AddStaticLightSource(
		{
			{ 1.0f,1.0f,0.0f },		// Light Position
			{ 1.0f,1.0f,1.0f,1.0f },// Light Color
			300.0f					// Light Intensity
		});

	m_ambient.AddStaticLightSource(
		{
			{ 800.0f/2,600.0f/2,0.0f },	// Light Position
			{ 1.0f,1.0f,1.0f,1.0f },	// Light Color
			500.0f						// Light Intensity
		});
	m_ambient.UpdateStaticLight(args.render.GetShader());

	Window::OnAttach(args);
}

void Game::OnUpdate(UpdateArgs args)
{
	test_movement(test_rect1, test_rect2, args);
	static glm::vec2 cp, cn;
	static float_t ht = 0;
	bool colide = colision::Colide::RayVsRect(test_rect1.pos, m_pos(args.mouse) - test_rect1.pos , test_rect2 ,cp, cn, ht);

		m_renderThisPlease.push_back( [=](RenderArgs& r_args)
		{
			r_args.render.DrawLine(this->test_rect1.pos + (this->test_rect1.size / 2.0f) , m_pos(args.mouse), { 1.0f,1.0f ,1.0f ,1.0f },2.0f,1.0f);
			if (colide && ht < 1.0f)
			{
				for (int i = 0; i < imInfo.ParticlesPer*2; i++)
				{
					imInfo.Effect.Position = cp;
					imInfo.Effect.LifeTime = imInfo.ParticlesLifeTime;
					imInfo.Effect.SizeBegin = 1.0f;
					imInfo.Effect.SizeEnd = 0.1f;
					imInfo.Effect.Gravity = 0.1f;
					m_emiter.Emit(imInfo.Effect);
				}
				r_args.render.DrawQuad(cp - (glm::vec2(5.0f)/2.0f), { 5.0f,5.0f }, { 1.0f,0.2f,0.2f,1.0f },1.0f);
			}
		});

	if (args.mouse.isPress(GLFW_MOUSE_BUTTON_2))
	{
		for (int i = 0; i < imInfo.ParticlesPer; i++)
		{
			imInfo.Effect.Position = m_pos(args.mouse) - (test_rect1.size / 2.0f);
			imInfo.Effect.LifeTime = imInfo.ParticlesLifeTime;
			imInfo.Effect.SizeBegin = 50.0f;
			imInfo.Effect.SizeEnd = 2.0f;
			m_emiter.Emit(imInfo.Effect);
		}
	}
	m_emiter.OnUpdate(args);
	test_rect1.pos += test_rect1.velocity * args.dt;

	Window::OnUpdate(args);
}

void Game::OnRender(RenderArgs args)
{
	//auto& render = args.render;
	args.render.DrawQuad(test_rect1.pos, test_rect1.size, m_animator.Run(args.dt));
	args.render.DrawQuad(test_rect2.pos, test_rect2.size, imInfo.color2);
	args.render.DrawQuad(glm::vec2(-600.0f), glm::vec2(800.0f,600.0f) * 8.0f, imInfo.color3);
	m_emiter.OnRender(args);
	for (auto& lamb : m_renderThisPlease)
		lamb(args);
	m_renderThisPlease.clear();
	
	Window::OnRender(args);
}

void Game::OnImGui(ImGuiArgs args)
{
	MainMenuBar(args);
	static float_t sc = 1000.0f;
	ImGui::Text("Quad Colors");
		ImGui::ColorEdit4("Moving Quad", &imInfo.color1.x);
		ImGui::ColorEdit4("Static Quad", &imInfo.color2.x);
		ImGui::ColorEdit4("BackGround Quad", &imInfo.color3.x);
	ImGui::Text("Particles Control");
		ImGui::ColorEdit4("Color Begin", &imInfo.Effect.ColorBegin.x);
		ImGui::ColorEdit4("Color End", &imInfo.Effect.ColorEnd.x);
		ImGui::SliderInt("Particles per time", &imInfo.ParticlesPer ,1, 100);
		ImGui::SliderFloat("Particle Life Time", &imInfo.ParticlesLifeTime, 0.01f, 100.0f);
	ImGui::Text("Light Control");
		ImGui::SliderFloat("Ambient Light", &sc, 0.0f, 1.0f);
		ImGui::SliderFloat("Middle Light Intencity", &imInfo.light1It,0.0f,1000.0f);
		ImGui::SliderFloat("0x0 Light Intencity",	&imInfo.light2It,0.0f,1000.0f);
		//if (ImGui::Button("Apply Light Change"))
		//{
			imInfo.ambientLight = { sc,sc,sc };
			m_ambient.UpdateAmbient(imInfo.ambientLight,args.render.GetShader(),false);
			m_ambient[0].m_LightIntencity = imInfo.light2It;
			m_ambient[1].m_LightIntencity = imInfo.light1It;
			m_ambient.UpdateStaticLight(args.render.GetShader());
		//}

		ImGui::Text("Audio");

		ImGui::SliderFloat("Global Gain", &imInfo.gGain, 0.0f, 1.0f);
		ImGui::SliderFloat("Global Pitch", &imInfo.gPitch, 0.0f, 10.0f);

		for (auto& audio : m_Audios) //audio is a pair
		{
			//Update global gain and pitch for testing
			audio.second.SetGain(imInfo.gGain);
			audio.second.SetPitch(imInfo.gPitch);

			const std::string button_label = "Play " + audio.first;
			if (ImGui::Button(button_label.c_str()))
				audio.second.Play();
		}



	Window::OnImGui(args);
}

void Game::Dispose()
{
	m_Textures.clear();

	Window::Dispose();
}