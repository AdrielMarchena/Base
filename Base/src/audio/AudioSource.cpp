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

//#include "Al_Error.h"

#define MY_AL_CHECK_ERROR(error,message) \
		{if (error = alGetError() != AL_NO_ERROR)\
		{\
			const std::string errorm = message + std::string(", AL ERROR: ") + alGetString(error);\
			throw std::exception(errorm.c_str());\
		}\
		const std::string errorm = message;\
		throw std::exception(errorm.c_str());}\

namespace en
{
namespace aux
{

	static inline void CreateAudio(std::unordered_map<std::string, AudioSource>& tmp ,utils::ResourceLoads<std::string, ALuint>& loads, const utils::NameCaps& nameCaps)
	{
		using namespace utils;
		while (!loads.isAllLoad())
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
						case NameCaps::NONE: break;
						case NameCaps::ALL_LOWER: name = utils::ToLower(name); break;
						case NameCaps::ALL_UPPER: name = utils::ToUpper(name); break;
						default: break;
						}
						tmp[name] = AudioSource(inf.second);
						inf.second = 0;
						loads.futures.erase(inf.first);
						loads.resources.erase(inf.first);
						LOG_NORMAL(inf.first + " AudioSource created!");
					}
					catch (const std::exception& ex)
					{
						const std::string error_what = ex.what();
						LOG_WARNING(inf.first << " could not be used to create AudioSorce\nError: " + error_what);
					}
					break;
				}
			}
		}
	}

	std::unordered_map<std::string, AudioSource> AudioSource::LoadAsyncAudios(const std::vector<std::pair<std::string, std::string>>& _NameFile, const utils::NameCaps& nameCaps, uint8_t batchLimit)
	{
		utils::ResourceLoads<std::string, ALuint> loads;
		auto lamb = [&](const std::string& name, const std::string& path)
		{
			auto info = LoadSoundEffect(path.c_str());
			if (info)
			{
				loads.resources[name] = info;
				LOG_NORMAL("sound: '" << name << "' Loaded!");
			}
			else
				LOG_NORMAL("sound: '" << name << "' Can't be loaded");
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
			try
			{
				auto info = LoadSoundEffect(f.second.c_str());
				if (info)
				{
					LOG_NORMAL("sound: '" << f.first << "' Loaded!");
				}
				else
				{
					LOG_NORMAL("sound: '" << f.first << "' Can't be loaded");
					throw std::exception("Buffer not created!");
				}
				tmp[f.first] = AudioSource(info);
				LOG_NORMAL(f.first + " AudioSource created!");
			}
			catch (const std::exception& ex)
			{
				const std::string error_what = ex.what();
				LOG_WARNING(f.first << " could not be used to create AudioSorce\nError: " + error_what);
			}
		}
		return tmp;
	}

	AudioSource::AudioSource(ALuint Buffer, float gain)
	{
		alGetError(); //Clear errors
		ALenum error = 0;
		p_Gain = gain;
		alGenSources(1, &p_Source);
		
		if(!p_Source)
			MY_AL_CHECK_ERROR(error,"Could not create source ID");

		alSourcef(p_Source, AL_PITCH, p_Pitch);
		alSourcef(p_Source, AL_GAIN, p_Gain);
		alSource3f(p_Source, AL_POSITION, p_Position[0], p_Position[1], p_Position[2]);
		alSource3f(p_Source, AL_VELOCITY, p_Velocity[0], p_Velocity[1], p_Velocity[2]);
		alSourcei(p_Source, AL_LOOPING, p_LoopSound);

		p_Buffer = Buffer;
		alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
		if (error = alGetError() != AL_NO_ERROR)\
		{\
			const std::string errorm = std::string("Something went wrong on alSourcei: ") + alGetString(error); \
			throw std::exception(errorm.c_str()); \
		}\
	}

	AudioSource::~AudioSource()
	{
		if (p_Source)
		{
			alSourceStop(p_Source);
			alDeleteSources(1, &p_Source);
		}
		if(p_Buffer)
			alDeleteBuffers(1, &p_Buffer);
	}

	void AudioSource::Play()
	{
		if (p_Source)
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
		if (p_Source)
			alSourcei(p_Source, AL_LOOPING, p_LoopSound);
	}

	void AudioSource::SetGain(float gain)
	{
		p_Gain = std::clamp(gain, gMin_gain, gMax_gain);
		if (p_Source)
			alSourcef(p_Source, AL_GAIN, p_Gain);
	}

	void AudioSource::SetPitch(float pitch)
	{
		p_Pitch = std::clamp(pitch,gMin_pitch,gMax_pitch);
		if (p_Source)
			alSourcef(p_Source, AL_PITCH, p_Pitch);
	}

	bool AudioSource::IsPlaying()
	{
		ALint state;
		if (p_Source)
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