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
	LoadTextures("test_imgs");
	LoadSounds("test_audio");

	Window::OnAttach(args);
}

void Game::OnUpdate(UpdateArgs args)
{

	Window::OnUpdate(args);
}

void Game::OnRender(RenderArgs args)
{
	
	Window::OnRender(args);
}

void Game::OnImGui(ImGuiArgs args)
{
	MainMenuBar(args);

	for (auto& audio : m_Audios)
		if (ImGui::Button(audio.first.c_str()))
			audio.second.Play();

	Window::OnImGui(args);
}

void Game::Dispose()
{
	m_Textures.clear();
	m_Audios.clear();
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
