#pragma once

#include "args/VirtualArgs.h"
#include "glm/glm.hpp"
#include <vector>

struct Back_Spec
{
	float_t b_threshold = 100.0f;// 100 default
	float_t b_velocity = 100.0f;// 100 default

	float_t m_threshold = 100.0f;// 100 default
	float_t m_velocity = 100.0f;// 100 default

	float_t f_threshold = 100.0f;// 100 default
	float_t f_velocity = 100.0f;// 100 default
};
class Background
{
private:
	float_t backlayer_threshold   = 0.0f;
	float_t middlelayer_threshold = 0.0f;
	float_t frontlayer_threshold  = 0.0f;

	std::vector<glm::vec3> back_pos;
	std::vector<glm::vec3> middle_pos;
	std::vector<glm::vec3> front_pos;

	en::render::Texture& m_SunTex;
	en::render::Texture& m_CloudTex;
public:

	Back_Spec m_Specs;
	
	Background()
		:m_SunTex(en::render::Texture::GetBlanckTexture()),
		 m_CloudTex(en::render::Texture::GetBlanckTexture())
	{}
	Background(const Back_Spec& spec)
		:m_Specs(spec),
		 m_SunTex(en::render::Texture::GetBlanckTexture()),
		 m_CloudTex(en::render::Texture::GetBlanckTexture())
	{}
	~Background(){}

	void OnAttach(const en::AttachArgs& args);
	void OnUpdate(const en::UpdateArgs& args);
	void OnRender(const en::RenderArgs& args);
};