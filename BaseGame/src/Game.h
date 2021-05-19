#pragma once

#include <functional>

#include "Window.h"
#include "colision/Colisions.h"
#include "render/ParticleSystem.h"
#include "amb/Ambient.h"
#include "ent/Animator.h"
#include "audio/AudioSource.h"
using namespace en;

struct ImGuiInfoPack
{
	glm::vec4 color1 = { 0.3f,0.6f,0.9f,1.0f };
	glm::vec4 color2 = { 0.9f,0.6f,0.3f,1.0f };
	glm::vec4 color3 = { 0.0f,0.5f,0.9f,1.0f };
	glm::vec3 ambientLight = windowing::Ambient::Day;
	float_t light1It = 1.0f;
	float_t light2It = 1.0f;
	render::ParticleProps Effect = render::ParticleProps::Effects[0];
	int32_t ParticlesPer = 10;
	float_t ParticlesLifeTime = 10;

};

class Game: public windowing::Window
{
protected:
	std::unordered_map<std::string, render::Texture> m_Textures;
	std::unordered_map<std::string, render::SubTexture> m_SubTextures;

	std::unordered_map<std::string, aux::AudioSource> m_Audios;

	std::vector<std::function<void(RenderArgs& r_args)>> m_renderThisPlease;

	Rect test_rect1;
	Rect test_rect2;

	ett::Animator m_animator;

	windowing::Ambient m_ambient;

	ImGuiInfoPack imInfo;
	render::ParticleSystem m_emiter;
public:

	Game(const char* title, float_t w = 800, float_t h = 600, bool resizeble = true);

	void OnAttach(AttachArgs args) override;
	void OnUpdate(UpdateArgs args) override;
	void OnRender(RenderArgs args) override;
	void OnImGui(ImGuiArgs args) override;
	void Dispose() override;
	
	//callback things

};

