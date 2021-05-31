#pragma once

#include <functional>

#include "Window.h"
#include "render/ParticleSystem.h"
#include "amb/Ambient.h"
#include "audio/AudioSource.h"
#include "Cell.h"
using namespace en;

struct ImGuiInfoPack
{
};

class Game: public windowing::Window
{
protected:
	std::unordered_map<std::string, render::Texture> m_Textures;
	std::unordered_map<std::string, render::SubTexture> m_SubTextures;
	std::unordered_map<std::string, aux::AudioSource> m_Audios;
	/* You can "ask" to render something for you from other funtions than OnRender */
	std::vector<std::function<void(RenderArgs& r_args)>> m_RenderThisPlease;

	windowing::Ambient m_Ambient;

	ImGuiInfoPack m_ImInfo;
	Map CellGame;
public:

	Game(const char* title, float_t w = 800, float_t h = 600, bool resizeble = true);

	void OnAttach(AttachArgs args) override;
	void OnUpdate(UpdateArgs args) override;
	void OnRender(RenderArgs args) override;
	void OnImGui(ImGuiArgs args) override;
	void Dispose() override;

	//callback things
	//
private:
	void LoadTextures(const char* directory);
	void LoadSounds(const char* directory);

};

