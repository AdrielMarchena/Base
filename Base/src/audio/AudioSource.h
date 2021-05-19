/*
*	@file AudioSource.h
*	@author Adriel Marchena Santos
*	
*	kind of a audio player, use things here to play audios
*/

#pragma once

#include <string>
#include <unordered_map>
#include "AL/al.h"

namespace en
{
namespace aux
{

	class AudioSource
	{
	public:
		AudioSource(ALuint Source,float gain = 1.0f);

		AudioSource(AudioSource& other) = delete;
		AudioSource(AudioSource&& other) noexcept
		{
			p_Source    =	other.p_Source;
			p_Pitch     =	other.p_Pitch;
			p_Gain		=	other.p_Gain;
			p_Position[0] = other.p_Position[0];
			p_Position[1] = other.p_Position[1];
			p_Position[2] = other.p_Position[2];
			p_Velocity[0] = other.p_Velocity[0];
			p_Velocity[1] = other.p_Velocity[1];
			p_Velocity[2] = other.p_Velocity[2];
			p_Buffer    =	other.p_Buffer;
			p_LoopSound =	other.p_LoopSound;

			other.p_Source = 0;
			other.p_Pitch = 0;
			other.p_Gain = 0;
			other.p_Position[0] = 0;
			other.p_Position[1] = 0;
			other.p_Position[2] = 0;
			other.p_Velocity[0] = 0;
			other.p_Velocity[1] = 0;
			other.p_Velocity[2] = 0;
			other.p_Buffer = 0;
			other.p_LoopSound = 0;
		}

		AudioSource& operator=(AudioSource& other) = delete;
		AudioSource& operator=(AudioSource&& other) noexcept
		{
			if (this == &other)
				return *this;

			alSourceStop(p_Source);
			alDeleteSources(1, &p_Source);
			alDeleteBuffers(1, &p_Buffer);

			p_Source = other.p_Source;
			p_Pitch = other.p_Pitch;
			p_Gain = other.p_Gain;
			p_Position[0] = other.p_Position[0];
			p_Position[1] = other.p_Position[1];
			p_Position[2] = other.p_Position[2];
			p_Velocity[0] = other.p_Velocity[0];
			p_Velocity[1] = other.p_Velocity[1];
			p_Velocity[2] = other.p_Velocity[2];
			p_Buffer = other.p_Buffer;
			p_LoopSound = other.p_LoopSound;

			other.p_Source = 0;
			other.p_Pitch = 0;
			other.p_Gain = 0;
			other.p_Position[0] = 0;
			other.p_Position[1] = 0;
			other.p_Position[2] = 0;
			other.p_Velocity[0] = 0;
			other.p_Velocity[1] = 0;
			other.p_Velocity[2] = 0;
			other.p_Buffer = 0;
			other.p_LoopSound = 0;

			return *this;
		}

		AudioSource(){}
		~AudioSource();
		
		/**
		* Play the audio
		*/
		void Play();
		void Loop(bool loop);
		void SetGain(float gain);
		bool IsPlaying();
		ALuint GetBuffer();
		ALuint GetSource();
		/**
		* Load audio files and Create Asynchronously a bunch of instances of this class
		*/
		static std::unordered_map<std::string, AudioSource> LoadAsyncAudios(const std::vector<std::pair<std::string, std::string>>& _NameFile, bool _Wait = true);
	private:
	   ALuint p_Source;
		float p_Pitch = 1.f;
		float p_Gain = 1.f;
		float p_Position[3] = { 0,0,0 };
		float p_Velocity[3] = { 0,0,0 };
		bool  p_LoopSound = false;
	   ALuint p_Buffer = 0;
	};


}
}