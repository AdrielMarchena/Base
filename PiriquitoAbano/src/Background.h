#pragma once

#include "args/VirtualArgs.h"
#include "glm/glm.hpp"
#include <vector>
#include <array>
#include <functional>
#include "glm/gtx/compatibility.hpp"

#include "utils/RandomList.h"

static constexpr float init_w_pos = 900.0f;
static constexpr float init_h_pos = 0.0f;

struct Back_Spec
{
	float_t b_threshold = 350.0f; // 350 default
	float_t b_velocity = 100.0f;  // 100 default
	float_t b_height = 450.0f;
	float_t b_thick = 550.0f;

	float_t m_threshold = 450.0f; // 450 default
	float_t m_velocity = 150.0f;  // 150 default
	float_t m_height = 250.0f;
	float_t m_thick = 300.0f;

	float_t f_threshold = 550.0f; // 550 default
	float_t f_velocity = 250.0f;  // 250 default
	float_t f_height = 100.0f;
	float_t f_thick = 200.0f;
};

class Mountain
{
public:
	glm::vec2 Points[3];
	glm::vec4 Color[3];
	bool alive = false;
	uint8_t Mountain_type;
private:
	static float b_layer;
	static float m_layer;
	static float f_layer;
	float& m_mylayer = b_layer;
	float m_myvel = 0;
public:

	Mountain()
	{
		static bool once = [&]()
		{
			b_layer = 0.2f;
			m_layer = 0.3f;
			f_layer = 0.4f;
			return true;
		}();
	}

	void MountMointain(float_t w, float_t h,uint8_t mountain_type,const Back_Spec& specs)
	{
		char pos_neg = 1;
		if (P_random() > 256 / 2)
			pos_neg = -1;
		alive = true;
		Points[0] = { init_w_pos, init_h_pos};
		Points[1] = { init_w_pos + (w / 2) + ((P_random() % 100) * pos_neg), h + ((P_random()) * pos_neg) };
		Points[2] = { init_w_pos + w + (P_random()), init_h_pos};
		Mountain_type = mountain_type;
		if (Mountain_type == 1u)
		{
			m_mylayer = b_layer;
			m_myvel = specs.b_velocity + (P_random() << 1);
		}
		if (Mountain_type == 2u)
		{
			m_mylayer = m_layer;
			m_myvel = specs.m_velocity + (P_random() << 1);
		}
		if (Mountain_type == 3u)
		{
			m_mylayer = f_layer;
			m_myvel = specs.f_velocity + (P_random() << 1);
		}
	}

	void Die()
	{
		alive = false;
		Points[0] = { -5000,-5000 };
		Points[1] = { -5000,-5000 };
		Points[2] = { -5000,-5000 };
	}

	void OnRender(const en::RenderArgs& args) const
	{
		//args.render.DrawCurveLine()
		if(alive)
			args.render.DrawTriangle(Points, Color ,(1 + m_mylayer));
	}
};


class Background
{
private:
	float_t backlayer_threshold   = 0.0f;
	float_t middlelayer_threshold = 0.0f;
	float_t frontlayer_threshold  = 0.0f;

	std::vector<Mountain> back_pos;
	std::vector<Mountain> middle_pos;
	std::vector<Mountain> front_pos;

	en::render::Texture* m_SunTex;
	en::render::Texture* m_CloudTex;
public:

	Back_Spec m_Specs;
	
	enum : uint8_t
	{
		SUN = 1u,
		CLOUD
	};

	enum : uint8_t
	{
		BACK_MOUNTAIN = 1u,
		MIDDLE_MOUNTAIN,
		FRONT_MOUNTAIN,
		ALL
	};

	Background()
		:m_SunTex(&en::render::Texture::GetBlanckTexture()),
		 m_CloudTex(&en::render::Texture::GetBlanckTexture())
	{}
	Background(const Back_Spec& spec)
		:m_Specs(spec),
		 m_SunTex(&en::render::Texture::GetBlanckTexture()),
		 m_CloudTex(&en::render::Texture::GetBlanckTexture())
	{}
	~Background(){}

	void OnAttach(const en::AttachArgs& args);
	void OnUpdate(const en::UpdateArgs& args);
	void OnRender(const en::RenderArgs& args);

	void SetTex(en::render::Texture* texture,uint8_t wich);

private:
	inline void MapM(std::function<bool(Mountain&)> lamb,uint8_t mountain_type = ALL);
};