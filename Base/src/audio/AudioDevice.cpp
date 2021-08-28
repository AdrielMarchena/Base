/*
*	@file AudioDevice.cpp
*	@author Adriel Marchena Santos
*	
*	Ambient system wich provide a simple way to insert
*	ambient light and source lights in a shader
*
*	Implementation File
*/

#include "AudioDevice.h"

#include <exception>
#include <stdio.h>
#include "AL/al.h"
#include "AL/alc.h"
#include "Log.h"
namespace en
{
namespace aux
{
	static ALCdevice* p_ALCDevice = nullptr;
	static ALCcontext* p_ALCContext = nullptr;

	void LoadDevices()
	{
		p_ALCDevice = alcOpenDevice(nullptr); // nullptr = get default device
		if (!p_ALCDevice)
			throw("failed to get sound device");

		p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);  // create context
		if (!p_ALCContext)
			throw("Failed to set sound context");

		if (!alcMakeContextCurrent(p_ALCContext))   // make context current
			throw("failed to make context current");

		const ALCchar* name = nullptr;
		if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
			name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
		if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
			name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);

		BASE_TRACE("Opened {0}\n", name);
		//printf("Opened \"%s\"\n", name);
	}

	ALCdevice* GetDevicePtr()
	{
		return p_ALCDevice;
	}
	ALCcontext* GetContextPtr()
	{
		return p_ALCContext;
	}

	void DeleteDevices()
	{
		if (!alcMakeContextCurrent(nullptr))
			throw std::exception("failed to set context to nullptr");

		alcDestroyContext(p_ALCContext);
		if (p_ALCContext)
			throw std::exception("failed to unset during close");

		if (!alcCloseDevice(p_ALCDevice))
			throw std::exception("failed to close sound device");
	}
}
}