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
#include <optional>
namespace Base
{
namespace ett
{
	class Animator;
	struct AnimationSpecs
	{
		enum : uint8_t
		{
			ANIMATOR_DEFAULT = NULL,
			ANIMATOR_SLOW,
			ANIMATOR_SLOW2,
			ANIMATOR_FAST,
			ANIMATOR_FAST2
		};

		float_t threshold = 350.0f; // if 0, set to a default value of 350
		float_t timestamp = 0.0f; // If 0, calculate based on threshold and the atlas
		float_t decrement = 5.0f; // if 0, default to 5
		uint32_t texture_offset = 0; // default to 0
		glm::vec2 sprite_size = {64.0f,64.0f}; //if 0, default to 64x64
		Base::Ref<Base::render::Texture> atlas; // Atlas Texture
		AnimationSpecs();
		//AnimationSpecs(Base::Ref<Base::render::Texture> texture);
		static AnimationSpecs CreateAnimationSpecs(Base::Ref<Base::render::Texture> texture,uint8_t preset = ANIMATOR_DEFAULT);
		operator const Animator*() { return m_Animator; }
	private:
		Animator* m_Animator = nullptr; // TODO: prevent some pointer bugs here
		friend Animator;
	};
	class Animator
	{
	private:
		AnimationSpecs m_Specs;
		float_t m_CurrentThreshold = 0.0f;
		float_t m_CurrentTimeStamp = 0.0f;
		uint32_t m_CurrentTexIndex = 0;
		std::vector<render::SubTexture> m_CroppedTexture;
		bool m_Stop = false;

		static constexpr float_t default_threshold = 350.0f;
		static constexpr float_t default_decrement = 5.0f;
	public:
		bool loop = true;
		
		Animator(const AnimationSpecs& _Specs);
		Animator();
		
		AnimationSpecs& GetSpecs() { return m_Specs; }
		
		// implicit convertion from Animator to AnimationSpecs
		operator AnimationSpecs&() { return m_Specs; }
		operator bool() { return m_CroppedTexture.size() != 0; }
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
		/**
		* Set a new Threshold to the animation
		*/
		inline void SetThreshold(float threshold) { m_Specs.threshold = threshold; }
		/**
		* Set a new decrement value to the threshold
		*/
		inline void SetDecrement(float decrement) { m_Specs.decrement = decrement; }
		/*
		* Stop Updating the Animation
		*/
		inline void StopUpdate() { m_Stop = true; }
		/*
		* Begin Updating the Animation
		*/
		inline void BeginUpdate() { m_Stop = false; }
	};
}
}


