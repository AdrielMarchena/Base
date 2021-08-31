#pragma once
#include "Window.h"
#include "args/VirtualArgs.h"
#include "Piriquito.h"
#include "Pipe.h"
#include "Piper.h"

#include <functional>

#include "render/Texture.h"
#include "render/Text.h"
#include "audio/AudioSource.h"

#include "colision/Colisions.h"

#include "amb/Ambient.h"

#include "Menu.h"
#include "Background.h"
#include "ResourceManager.h"

enum class PiriquitoState : uint8_t
{
	NONE = 0,
	PLAYING,
	PAUSE,
	GAMEOVER
};

struct ImGuiT
{
};

class PiriquitoAbano : public en::windowing::Window
{
private:
	//std::unordered_map<std::string, en::render::Texture> m_Textures;
	ResourceManager<en::render::Texture> m_Textures;
	ResourceManager<en::render::Text> m_Text;
	ResourceManager<en::aux::AudioSource> m_Audios;

	en::windowing::Ambient m_Lights;

	Piriquito m_Piriquito;
	float_t m_Gravity = 9.8f;
	uint64_t points = 0;

	glm::vec2 m_Pointer[3];

	std::shared_ptr<en::render::Text> m_TextFont;

	PiriquitoState m_State = PiriquitoState::NONE;

	en::GUI::Menu GameOverBox;
	en::GUI::Menu StartBox;

	Piper m_Piper;
	Background m_Back;
public:
	PiriquitoAbano();
	~PiriquitoAbano(){}
protected:
	void OnAttach(en::AttachArgs args) override;
	void OnUpdate(en::UpdateArgs args) override;
	void OnRender(en::RenderArgs args) override;
	void OnImGui(en::ImGuiArgs args) override;
	void Dispose() override;

	virtual void OnMouseAction(en::MouseArgs args) override;

	void Restart(en::UpdateArgs args);
private:
	inline void SwitchPause();
	inline void LoadSounds(const char* directory);
	inline void LoadTextures(const char* directory);
	inline void LoadText(const char* directory);
};

