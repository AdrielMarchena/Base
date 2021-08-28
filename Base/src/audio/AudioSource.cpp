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
#include "utils/Generic.h"
#include <algorithm>
#include "utils/al_error_marcro_db.h"
#include "Log.h"

namespace en
{
namespace aux
{

	static inline void TryCreateAudio(std::unordered_map<std::string, AudioSource>& tmp, utils::ResourceLoads<std::string, ALuint>& loads, const utils::NameCaps& nameCaps)
	{
		for (auto& inf : loads.resources)
		{
			if (loads.futures[inf.first]._Is_ready())
			{
				try
				{
					if (!inf.second)
						throw std::exception("Buffer not created!");
					std::string name = inf.first;
					switch (nameCaps)
					{
					case utils::NameCaps::NONE: break;
					case utils::NameCaps::ALL_LOWER: name = utils::ToLower(name); break;
					case utils::NameCaps::ALL_UPPER: name = utils::ToUpper(name); break;
					default: break;
					}
					tmp[name] = AudioSource(inf.second);
					inf.second = 0;
					loads.futures.erase(inf.first);
					loads.resources.erase(inf.first);
					BASE_TRACE("{0} AudioSource created!", inf.first);
				}
				catch (const std::exception& ex)
				{
					const std::string error_what = ex.what();
					//TODO: This message is strange
					BASE_WARN("{0} could not be used to create AudioSorce\nError: '{1}'",inf.first, error_what);
				}
				break;
			}
		}
	}

	static inline void CreateAudio(std::unordered_map<std::string, AudioSource>& tmp ,utils::ResourceLoads<std::string, ALuint>& loads, const utils::NameCaps& nameCaps)
	{
		using namespace utils;
		while (!loads.isAllLoad())
		{
			std::lock_guard<std::mutex> lock(loads.mutex);
			TryCreateAudio(tmp, loads, nameCaps);
		}
		std::lock_guard<std::mutex> lock(loads.mutex);
		TryCreateAudio(tmp, loads, nameCaps);
		loads.futures.clear();
		loads.resources.clear();
	}

	std::unordered_map<std::string, AudioSource> AudioSource::LoadAsyncAudios(const std::vector<std::pair<std::string, std::string>>& _NameFile, const utils::NameCaps& nameCaps, uint8_t batchLimit)
	{
		utils::ResourceLoads<std::string, ALuint> loads;
		auto lamb = [&](const std::string& name, const std::string& path)
		{
			auto info = LoadSoundEffect(path.c_str());
			if (info)
			{
				std::lock_guard<std::mutex> lock(loads.mutex);
				loads.resources[name] = info;
				BASE_TRACE("sound: '{0}' Loaded!",name);
			}
			else
			{
				std::lock_guard<std::mutex> lock(loads.mutex);
				BASE_ERROR("sound: '{0}' Could not be loaded!", name);
			}
		};
		uint8_t count = 0;
		std::unordered_map<std::string, AudioSource> tmp;
		for (auto& name : _NameFile)
		{
			loads.futures[name.first] = std::async(std::launch::async, lamb, name.first, name.second);
			count++;
			if (count >= batchLimit)
			{
				CreateAudio(tmp,loads,nameCaps);
				loads.resources.clear();
				loads.futures.clear();
				count = 0;
			}
		}
		CreateAudio(tmp, loads,nameCaps);
		return tmp;
	}

	std::unordered_map<std::string, AudioSource> AudioSource::LoadAudios(const std::vector<std::pair<std::string, std::string>>& _NameFile, bool _Wait)
	{
		std::unordered_map<std::string, AudioSource> tmp;
		for (auto& f : _NameFile)
		{
			auto info = LoadSoundEffect(f.second.c_str());
			if (info)
			{
				BASE_TRACE("sound: '{0}' Loaded!", f.first);
			}
			else
			{
				BASE_ERROR("sound: '{0}' Could not be loaded!", f.first);
			}
			tmp[f.first] = AudioSource(info);
			BASE_TRACE("Audio: '{0}' Created!", f.first);
			
		}
		return tmp;
	}

	AudioSource::AudioSource(ALuint Buffer, float gain)
	{
		p_Gain = gain;
		ALCall(alGenSources(1, &p_Source));
		
		if (!p_Source)
		{
			BASE_ERROR("Could not generate SourceID");
			return;
		}

		ALCall(alSourcef(p_Source, AL_PITCH, p_Pitch));
		ALCall(alSourcef(p_Source, AL_GAIN, p_Gain));
		ALCall(alSource3f(p_Source, AL_POSITION, p_Position[0], p_Position[1], p_Position[2]));
		ALCall(alSource3f(p_Source, AL_VELOCITY, p_Velocity[0], p_Velocity[1], p_Velocity[2]));
		ALCall(alSourcei(p_Source, AL_LOOPING, p_LoopSound));

		p_Buffer = Buffer;
		ALCall(alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer));
	}

	AudioSource::~AudioSource()
	{
		if (p_Source)
		{
			ALCall(alSourceStop(p_Source));
			ALCall(alDeleteSources(1, &p_Source));
		}
		if(p_Buffer)
			ALCall(alDeleteBuffers(1, &p_Buffer));
	}

	void AudioSource::Play()
	{
		if (p_Source)
			ALCall(alSourcePlay(p_Source));

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
		if (p_Source)
			ALCall(alSourcei(p_Source, AL_LOOPING, p_LoopSound));
	}

	void AudioSource::SetGain(float gain)
	{
		p_Gain = std::clamp(gain, gMin_gain, gMax_gain);
		if (p_Source)
			ALCall(alSourcef(p_Source, AL_GAIN, p_Gain));
	}

	void AudioSource::SetPitch(float pitch)
	{
		p_Pitch = std::clamp(pitch,gMin_pitch,gMax_pitch);
		if (p_Source)
			ALCall(alSourcef(p_Source, AL_PITCH, p_Pitch));
	}

	bool AudioSource::IsPlaying()
	{
		ALint state = 0;
		if (p_Source)
			ALCall(alGetSourcei(p_Source, AL_SOURCE_STATE, &state));
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