/*
*	@file Animator.h
*	@author Adriel Marchena Santos
*	
*	Animation System
*
*/

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
		float_t threshold = 350.0f; // if 0, set to a default value of 350
		float_t timestamp = 0.0f; // If 0, calculate based on threshold and the atlas
		float_t decrement = 5.0f; // if 0, default to 5
		uint32_t texture_offset = 0; // default to 0
		glm::vec2 sprite_size = {64.0f,64.0f}; //if 0, default to 64x64
		render::Texture& atlas; // Atlas Texture

		AnimationSpecs();
		AnimationSpecs(render::Texture& texture);
		AnimationSpecs& operator=(AnimationSpecs& other);
	};
	class Animator
	{
	private:
		AnimationSpecs m_Specs;
		float_t m_CurrentThreshold = 0.0f;
		float_t m_CurrentTimeStamp = 0.0f;
		uint32_t m_CurrentTexIndex = 0;
		std::vector<render::SubTexture> m_CroppedTexture;

		static constexpr float_t default_threshold = 350.0f;
		static constexpr float_t default_decrement = 5.0f;
	public:
		bool loop = true;
		
		Animator(const AnimationSpecs& _Specs);
		Animator();
		
		const AnimationSpecs& GetSpecs() const { return m_Specs; }

		/**
		* Run the animation based on the specs, if the loop is false
		* it will return the last Frame from on
		*/
		render::SubTexture& Run(float_t dt);
		/**
		* Resets the animation to the values on the givin specs
		*/
		void Reset();
		/**
		* Set a new Animation specs, includin new Texture to Crop
		*/
		void SetNewTexture(const AnimationSpecs& _Specs);

	};
}
}


