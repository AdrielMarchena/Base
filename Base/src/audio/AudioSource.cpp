/*
*	@file AudioSource.cpp
*	@author Adriel Marchena Santos
*	
*	kind of a audio player, use things here to play audios
*
*	Implementation File
*/

#include "AudioSource.h"

#include <future>

#include "utils/threading.h"
#include "SoundBuffer.h"
#include "utils/Logs.h"
namespace en
{
namespace aux
{
	//TODO: It's probably better to store the source buffer on the map, either way, can still get the source buffer from the AudioSource
	std::unordered_map<std::string, AudioSource> AudioSource::LoadAsyncAudios(const std::vector<std::pair<std::string, std::string>>& _NameFile, bool _Wait)
	{
		utils::ResourceLoads<std::string, AudioSource> loads;
		auto lamb = [&](const std::string& name, const std::string& path)
		{
			auto info = LoadSoundEffect(path.c_str());
			if (info)
			{
				D_LOG("sound: '" << name << "' Loaded!");
				std::lock_guard<std::mutex> lock(loads.mutex); // Since the lock is here, there is not to much of a gain here
				loads.resources[name] = AudioSource(info);
			}
			else
				D_LOG("sound: '" << name << "' Can't be loaded");
		};

		for (auto& name : _NameFile)
		{
			loads.futures[name.first] = std::async(std::launch::async, lamb, name.first, name.second);
		}

		/*std::unordered_map<std::string, AudioSource> tmp;
		while (!loads.isAllLoad())
		{
			for (auto& inf : loads.resources)
			{
				if (loads.futures[inf.first]._Is_ready())
				{
					tmp[inf.first] = AudioSource(inf.second);
					inf.second = 0;
					loads.futures.erase(inf.first);
					loads.resources.erase(inf.first);
					break;
				}
			}
		}*/

		if (_Wait)
			loads.waitAll();

		return std::move(loads.resources);
	}

	AudioSource::AudioSource(ALuint Buffer, float gain)
	{
		p_Gain = gain;
		alGenSources(1, &p_Source);
		alSourcef(p_Source, AL_PITCH, p_Pitch);
		alSourcef(p_Source, AL_GAIN, p_Gain);
		alSource3f(p_Source, AL_POSITION, p_Position[0], p_Position[1], p_Position[2]);
		alSource3f(p_Source, AL_VELOCITY, p_Velocity[0], p_Velocity[1], p_Velocity[2]);
		alSourcei(p_Source, AL_LOOPING, p_LoopSound);

		p_Buffer = Buffer;
		alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
	}

	AudioSource::~AudioSource()
	{
		alSourceStop(p_Source);
		alDeleteSources(1, &p_Source);
		alDeleteBuffers(1, &p_Buffer);
	}

	void AudioSource::Play()
	{

		alSourcePlay(p_Source);

		/*ALint state = AL_PLAYING;
		std::cout << "playing sound\n";
		while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
		{
			std::cout << "currently playing sound\n";
			alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
		}
		std::cout << "done playing sound\n";*/
	}

	void AudioSource::Loop(bool loop)
	{
		p_LoopSound = loop;
		alSourcei(p_Source, AL_LOOPING, p_LoopSound);
	}

	void AudioSource::SetGain(float gain)
	{
		p_Gain = gain;
		alSourcef(p_Source, AL_GAIN, p_Gain);
	}

	bool AudioSource::IsPlaying()
	{
		ALint state;
		alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING)
			return true;
		return false;
	}

	ALuint AudioSource::GetBuffer()
	{
		return p_Buffer;
	}
	ALuint AudioSource::GetSource()
	{
		return p_Source;
	}
}
}