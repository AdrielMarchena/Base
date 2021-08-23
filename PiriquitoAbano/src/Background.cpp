#include "Background.h"

float Mountain::b_layer = 0.0f;
float Mountain::m_layer = 0.0f;
float Mountain::f_layer = 0.0f;

void Background::OnAttach(const en::AttachArgs& args)
{
	back_pos.resize(10);
	middle_pos.resize(15);
	front_pos.resize(20);

	back_pos.emplace_back();
	back_pos[0].MountMointain(m_Specs.b_height, m_Specs.b_thick, BACK_MOUNTAIN);

	middle_pos.emplace_back();
	middle_pos[0].MountMointain(m_Specs.m_height, m_Specs.m_thick, MIDDLE_MOUNTAIN);

	front_pos.emplace_back();
	front_pos[0].MountMointain(m_Specs.f_height, m_Specs.f_thick, FRONT_MOUNTAIN);
}

void Background::OnUpdate(const en::UpdateArgs& args)
{
	backlayer_threshold -=  200.0f * args.dt;
	middlelayer_threshold -=  200.0f * args.dt;
	frontlayer_threshold -=  200.0f * args.dt;

	if (backlayer_threshold <= 0)
	{
		MapM([&](Mountain& m)
		{
			if (!m.alive)
			{
				m.alive = true;
				m.MountMointain(m_Specs.b_height, m_Specs.b_thick, BACK_MOUNTAIN);
				m.Color[0] = {0.1f,0.9f,0.4f,1.0f };
				m.Color[1] = {0.6f,0.6f,0.6f,1.0f };
				m.Color[2] = m.Color[0];
				return true;
			}
			return false;
		}, BACK_MOUNTAIN);
		backlayer_threshold = m_Specs.b_threshold;
	}
	
	if (middlelayer_threshold <= 0)
	{
		MapM([&](Mountain& m)
		{
			if (!m.alive)
			{
				m.alive = true;
				m.MountMointain(m_Specs.m_height, m_Specs.m_thick, MIDDLE_MOUNTAIN);
				m.Color[0] = { 0.2f,0.8f,0.5f,1.0f };
				m.Color[1] = { 0.7f,0.7f,0.7f,1.0f };
				m.Color[2] = m.Color[0];
				return true;
			}
			return false;
		}, MIDDLE_MOUNTAIN);
		middlelayer_threshold = m_Specs.m_threshold;
	}
	if (frontlayer_threshold <= 0)
	{
		MapM([&](Mountain& m)
		{
			if (!m.alive)
			{
				m.alive = true;
				m.MountMointain(m_Specs.f_height, m_Specs.f_thick, FRONT_MOUNTAIN);
				m.Color[0] = { 0.3f,0.8f,0.6f,1.0f };
				m.Color[1] = { 0.8f,0.8f,0.8f,1.0f };
				m.Color[2] = m.Color[0];
				return true;
			}
			return false;
		}, FRONT_MOUNTAIN);
		frontlayer_threshold = m_Specs.f_threshold;
	}

	MapM([&](Mountain& m)
	{
		if (!m.alive)
			return false;

		float v = 0.0f;
		if(m.Mountain_type == BACK_MOUNTAIN)
			v = m_Specs.b_velocity;
			
		if(m.Mountain_type == MIDDLE_MOUNTAIN)
			v = m_Specs.m_velocity;
		
		if(m.Mountain_type == FRONT_MOUNTAIN)
			v = m_Specs.f_velocity;

		m.Points[0].x -= v * args.dt;
		m.Points[1].x -= v * args.dt;
		m.Points[2].x -= v * args.dt;

		if (m.Points[2].x <= -50.0f)
			m.Die();

		return false; 
	});
}

void Background::OnRender(const en::RenderArgs& args)
{
	static glm::vec4 b_color;
	static bool once = [&]()
	{
		float n = 1.0f / 256.0f;
		b_color = { n * 95.0f,n * 197.0f,n * 228.0f,1.0f };
		return true;
	}();

	args.render.DrawQuad({ 0.0f,0.0f }, { args.res_w ,args.res_h }, b_color, -0.0f);
	MapM([&](Mountain& m)
	{
		m.OnRender(args);
		return false;
	},ALL);
}

void Background::SetTex(en::render::Texture* texture, uint8_t wich)
{
	if (!texture)
		return;
	switch (wich)
	{
	case SUN:
		m_SunTex = texture;
		break;
	case CLOUD:
		m_CloudTex = texture;
		break;
	default:
		break;
	}
}

void Background::MapM(std::function<bool(Mountain&)> lamb, uint8_t mountain_type)
{
	switch (mountain_type)
	{
	case BACK_MOUNTAIN:
		for (auto& m : back_pos)
		{
			if (lamb(m))
				break;
		}
			break;
	case MIDDLE_MOUNTAIN:
		for (auto& m : middle_pos)
		{
			if (lamb(m))
				break;
		}
		break;
	case FRONT_MOUNTAIN:
		for (auto& m : front_pos)
		{
			if (lamb(m))
				break;
		}
		break;
	default:
	case ALL:
		for (auto& m : back_pos)
		{
			if (lamb(m))
				break;
		}
		for (auto& m : middle_pos)
		{
			if (lamb(m))
				break;
		}
		for (auto& m : front_pos)
		{
			if (lamb(m))
				break;
		}
		break;
	}
}
