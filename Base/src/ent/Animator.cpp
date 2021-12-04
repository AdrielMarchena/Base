/*
*	@file Animator.cpp
*	@author Adriel Marchena Santos
*	
*	Animation System
*
* 	Implementation File
*/
#include "pch.h"
#include "Animator.h"

#include <limits>

namespace Base
{
namespace ett
{
	static constexpr float_t default_x_sprite_size = 64.0f;
	static constexpr float_t default_y_sprite_size = 64.0f;
	static constexpr unsigned int max_uint32 = std::numeric_limits<uint32_t>::max();
	static Base::Ref<Base::render::Texture> default_white_texture = std::make_shared<Base::render::Texture>();
	static render::SubTexture default_sub_white_texture = render::SubTexture::CreateFromCoords(default_white_texture, { 1,1 }, { 0,0 }, { 1,1 });;
	
	AnimationSpecs::AnimationSpecs()
	{
	}

	AnimationSpecs AnimationSpecs::CreateAnimationSpecs(Base::Ref<Base::render::Texture> texture, uint8_t preset)
	{
		AnimationSpecs new_spec{};
		new_spec.atlas = texture;
		switch (preset)
		{
		case ANIMATOR_FAST:
			new_spec.decrement *= 1.5f;
			break;
		case ANIMATOR_FAST2:
			new_spec.decrement *= 2.0f;
			break;
		case ANIMATOR_SLOW:
			new_spec.decrement *= 0.5f;
			break;
		case ANIMATOR_SLOW2:
			new_spec.decrement *= 0.25f;
			break;
		default:
		case ANIMATOR_DEFAULT:
		break;
		}
		return new_spec;
	}

	Animator::Animator(const AnimationSpecs& _Specs)
	{
		SetNewTexture(_Specs);
	}

	Animator::Animator()
		:m_Specs()
	{
		m_Specs.atlas = default_white_texture;
		m_Specs.m_Animator = this;
	}

	render::SubTexture& Animator::Run(float_t dt)
	{
		size_t s = m_CroppedTexture.size();
		if (!s)
			return default_sub_white_texture;
		if (m_Stop)
			return m_CroppedTexture[m_CurrentTexIndex];

		if (m_CurrentTimeStamp <= 0)
		{
			//Go to next texture
			if(s)
				m_CurrentTexIndex = std::min((m_CurrentTexIndex + 1) % int(s), max_uint32);// TODO: need all this?
			m_CurrentTimeStamp = m_Specs.timestamp;
		}
		if (loop && m_CurrentThreshold <= 0 && m_CurrentTimeStamp <= 0)
		{
			Reset();
		}
		
		double_t delta_decrement = m_Specs.decrement * dt;
		m_CurrentTimeStamp -= delta_decrement;
		m_CurrentThreshold -= delta_decrement;
		return m_CroppedTexture[m_CurrentTexIndex];
	}
	void Animator::Reset()
	{
		int s = m_CroppedTexture.size();
		m_CurrentTexIndex = m_Specs.texture_offset % (s-1);
		m_CurrentThreshold = m_Specs.threshold;
		m_CurrentTimeStamp = m_Specs.timestamp;
	}
	void Animator::SetNewTexture(const AnimationSpecs& _Specs)
	{
		m_Specs = _Specs;
		m_Specs.m_Animator = this;
		glm::vec2 atlas_size = m_Specs.atlas->GetSize();

		if (m_Specs.threshold <= 0.0f)
			m_Specs.threshold = default_threshold;
		
		if (m_Specs.sprite_size.x == 0.0f)
			m_Specs.sprite_size.x = default_x_sprite_size;
		if (m_Specs.sprite_size.y == 0.0f)
			m_Specs.sprite_size.y = default_y_sprite_size;

		if (m_Specs.decrement <= 0.0f)
			m_Specs.decrement = default_decrement;

		//Create SubTextures
		float_t calc_size = atlas_size.x / m_Specs.sprite_size.x;

		if (calc_size <= 0.0f)
		{
			m_Specs.atlas = default_white_texture;
			calc_size = 1.0f;
		}

		for (int i = 0; i < calc_size; i++)
		{
			m_CroppedTexture.push_back(render::SubTexture::CreateFromCoords(
				m_Specs.atlas, atlas_size, { i,0.0f }, m_Specs.sprite_size)
			);
		}

		int s = m_CroppedTexture.size();
		m_CurrentTexIndex = m_Specs.texture_offset % (s - 1);
		if (m_Specs.timestamp <= 0)
			m_Specs.timestamp = m_Specs.threshold / s;

		if (m_Specs.texture_offset >= s)
			m_Specs.texture_offset = s - 1;

		Reset();
		//m_CurrentThreshold = m_Specs.threshold;
		//m_CurrentTimeStamp = m_Specs.timestamp;
		//m_CurrentTexIndex = 0;
	}
}
}