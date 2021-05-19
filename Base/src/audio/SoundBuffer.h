/*
*	@file SoundBuffer.h
*	@author Adriel Marchena Santos
*	
*	Audio and OpenAL Related things
*
*/

#pragma once
#include "AL/al.h"
#include <vector>

namespace en
{
namespace aux
{
	/**
	* Load the Sound effect and create a AL buffer to it (Or something like that)
	*/
	[[nodiscard("Don't throw away the audio buffer please")]]ALuint LoadSoundEffect(const char* filename);
}
}


