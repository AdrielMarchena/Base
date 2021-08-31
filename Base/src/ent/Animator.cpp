/*
*	@file Animator.cpp
*	@author Adriel Marchena Santos
*	
*	Animation System
*
* 	Implementation File
*/

#include "Animator.h"

#include "utils/Logs.h"

#include <limits>

namespace en
{
namespace ett
{
	static constexpr float_t default_x_sprite_size = 64.0f;
	static constexpr float_t default_y_sprite_size = 64.0f;
	static std::shared_ptr<en::render::Texture> default_white_texture = std::make_shared<render::Texture>();
	static render::SubTexture default_sub_white_texture = render::SubTexture::CreateFromCoords(*default_white_texture, { 1,1 }, { 0,0 }, { 1,1 });;
	
	AnimationSpecs::AnimationSpecs()
		:atlas(default_white_texture)
	{
	}
	AnimationSpecs::AnimationSpecs(std::shared_ptr<en::render::Texture> texture)
		:atlas(texture)
	{
	}

	AnimationSpecs& AnimationSpecs::operator=(AnimationSpecs& other)
	{
		if (this == &other)
			return *this;
		threshold = other.threshold;
		timestamp = other.timestamp;
		decrement = other.decrement;
		texture_offset = other.texture_offset;
		sprite_size = other.sprite_size;
		atlas = other.atlas;
		return *this;
	}

	Animator::Animator(const AnimationSpecs& _Specs)
	{
		SetNewTexture(_Specs);
	}

	Animator::Animator()
		:m_Specs()
	{
		m_Specs.atlas = default_white_texture;
		//default_white_texture = render::Texture(); //This is trash
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
				m_CurrentTexIndex = std::min((m_CurrentTexIndex + 1) % int(s), unsigned int(std::numeric_limits<uint32_t>::max()));
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
		m_CurrentTexIndex = m_Specs.texture_offset;
		m_CurrentThreshold = m_Specs.threshold;
		m_CurrentTimeStamp = m_Specs.timestamp;
	}
	void Animator::SetNewTexture(const AnimationSpecs& _Specs)
	{
		m_Specs = const_cast<AnimationSpecs&>(_Specs);
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
				*m_Specs.atlas, atlas_size, { i,0.0f }, m_Specs.sprite_size)
			);
		}

		if (m_Specs.timestamp <= 0)
			m_Specs.timestamp = m_Specs.threshold / m_CroppedTexture.size();

		if (m_Specs.texture_offset >= m_CroppedTexture.size())
			m_Specs.texture_offset = m_CroppedTexture.size() - 1;

		m_CurrentThreshold = m_Specs.threshold;
		m_CurrentTimeStamp = m_Specs.timestamp;
		m_CurrentTexIndex = 0;
	}
}
}