#pragma once
#include "Window.h"
#include "args/VirtualArgs.h"
#include "Piriquito.h"
#include "Pipe.h"
#include "Piper.h"

#include "glm/gtx/compatibility.hpp"
#include "render/Texture.h"
#include "render/Text.h"
#include "audio/AudioSource.h"

#include "colision/Colisions.h"

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

struct Menu
{
	en::Rect out_box;
	en::Rect button;
	const char* out_text;
	const char* button_text;

	glm::vec3 out_text_color;
	glm::vec3 button_text_color;
	glm::vec3 hover_button_text_color;
	glm::vec3* actual_button_text_color = &button_text_color;

	void OnUpdate(const en::UpdateArgs& args)
	{
		
	}
	void OnRender(const en::RenderArgs& args,const en::render::Text& font)
	{
		args.render.DrawQuad(out_box.pos, out_box.size, {0.2f,0.5f,0.9f,0.5f}, 3.0f);
		font.RenderText(args, out_text, glm::lerp(out_box.pos.x, out_box.pos.x + out_box.size.x,0.33f), glm::lerp(out_box.pos.y, out_box.pos.y + out_box.size.y, 0.70f), 0.50f,{ 1.0f,1.0f,1.0f });

		button.pos.x = (out_box.pos.x + (out_box.size.x / 2)) - button.size.x / 2;
		button.pos.y = glm::lerp(out_box.pos.y, out_box.pos.y + out_box.size.y, 0.05f);

		args.render.DrawQuad(button.pos, button.size, { 0.2f,0.6f,0.2f,0.5f }, 3.0f);
		font.RenderText(args, button_text, glm::lerp(button.pos.x, button.pos.x + button.size.x, 0.05f), glm::lerp(button.pos.y, button.pos.y + button.size.y, 0.33f), 0.45f, *actual_button_text_color);
	}
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

	glm::vec2 m_Pointer[3];

	PiriquitoState m_State = PiriquitoState::NONE;

	Menu GameOverBox;

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

	virtual void OnMouseAction(en::MouseArgs args) override;

	void Restart(en::UpdateArgs args);
private:
	inline void SwitchPause();
	inline void LoadSounds(const char* directory);
	inline void LoadTextures(const char* directory);
	inline void LoadText(const char* directory);
};

