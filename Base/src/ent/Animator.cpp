#include "Animator.h"



namespace en
{
namespace ett
{
	Animator::Animator(const AnimationSpecs& _Specs)
	{
		SetNewTexture(_Specs);
	}

	render::SubTexture& Animator::Run(float_t dt)
	{
		if (current_timestamp <= 0)
		{
			//Go to next texture
			current_tex_index = (current_tex_index + 1) % m_CroppedTexture.size();
			current_timestamp = specs.timestamp;
		}

		if (loop && current_threshold <= 0 && current_timestamp <= 0)
		{
			Reset();
		}
		
		double_t delta_decrement = specs.decrement * dt;
		current_timestamp -= delta_decrement;
		current_threshold -= delta_decrement;
		return m_CroppedTexture[current_tex_index];
	}
	void Animator::Reset()
	{
		current_tex_index = specs.texture_offset;
		current_threshold = specs.threshold;
		current_timestamp = specs.timestamp;
	}
	void Animator::SetNewTexture(const AnimationSpecs& _Specs)
	{
		specs = _Specs;
		glm::vec2 atlas_size = specs.atlas->GetSize();

		if (specs.threshold <= 0)
			specs.threshold = default_threshold;
		
		if (specs.sprite_size == glm::vec2(0))
			specs.sprite_size = { 64.0f,64.0f };

		if (specs.decrement <= 0)
			specs.decrement = default_decrement;

		//Create SubTextures
		float_t calc_size = atlas_size.x / specs.sprite_size.x;
		for (int i = 0; i < calc_size; i++)
		{
			m_CroppedTexture.push_back(render::SubTexture::CreateFromCoords(
				specs.atlas, atlas_size, { i,0.0f }, specs.sprite_size)
			);
		}

		if (specs.timestamp <= 0)
			specs.timestamp = specs.threshold / m_CroppedTexture.size();

		if (specs.texture_offset < 0) //Kinda redundant, this is unsigned
			specs.texture_offset = 0;
		if (specs.texture_offset >= m_CroppedTexture.size())
			specs.texture_offset = m_CroppedTexture.size() - 1;

		current_threshold = specs.threshold;
		current_timestamp = specs.timestamp;
		current_tex_index = 0;
	}
}
}