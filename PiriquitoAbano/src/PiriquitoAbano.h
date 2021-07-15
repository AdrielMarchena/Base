#pragma once
#include "Window.h"
#include "args/VirtualArgs.h"

#include "Piriquito.h"
#include "Pipe.h"
#include "Piper.h"

#include "render/Texture.h"
#include "render/Text.h"
#include "audio/AudioSource.h"

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
	std::unordered_map<std::string, en::render::Texture> m_Textures;
	std::unordered_map<std::string, en::render::Text> m_Text;
	std::unordered_map<std::string, en::aux::AudioSource> m_Audios;

	Piriquito m_Piriquito;
	float_t m_Gravity = 9.8f;
	uint64_t points = 0;

	PiriquitoState m_State = PiriquitoState::NONE;

	Piper m_Piper;
public:
	PiriquitoAbano();
	~PiriquitoAbano(){}
protected:
	void OnAttach(en::AttachArgs args) override;
	void OnUpdate(en::UpdateArgs args) override;
	void OnRender(en::RenderArgs args) override;
	void OnImGui(en::ImGuiArgs args) override;
	void Dispose() override;

	void Restart(en::UpdateArgs args);
private:
	inline void LoadSounds(const char* directory);
	inline void LoadTextures(const char* directory);
	inline void LoadText(const char* directory);
};
