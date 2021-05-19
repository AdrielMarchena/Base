#pragma once
#include <stdint.h>
#include <vector>
#include "glm/glm.hpp"
#include "render/Texture.h"
#include "render/SubTexture.h"

namespace en
{
namespace ett
{
	struct AnimationSpecs
	{
		int32_t threshold; // if 0, set to a default value of 350
		int32_t timestamp; // If 0, calculate based on threshold and the atlas
		int32_t decrement; // if 0, default to 5
		uint32_t texture_offset; // if less 0, default to 0
		render::Texture* atlas; // Atlas Texture
		glm::vec2 sprite_size; //if 0, default to 64x64

	};
	class Animator
	{
	private:
		AnimationSpecs specs;
		int32_t current_threshold = 0;
		int32_t current_timestamp = 0;
		int32_t current_tex_index = 0;
		std::vector<render::SubTexture> m_CroppedTexture;

		const static int default_threshold = 350;
		const static int default_decrement = 5;
	public:
		bool loop = true;
		
		Animator(const AnimationSpecs& _Specs);
		Animator(){}

		render::SubTexture& Run(float_t dt);
		void Reset();
		void SetNewTexture(const AnimationSpecs& _Specs);

	};
}
}


