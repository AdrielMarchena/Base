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
#include "utils/alc_error_macro_db.h"
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

		ALCCall(p_ALCContext = alcCreateContext(p_ALCDevice, nullptr), p_ALCDevice);  // create context
		if (!p_ALCContext)
			throw("Failed to set sound context");

		ALCCall(if (!alcMakeContextCurrent(p_ALCContext))   // make context current
			throw("failed to make context current");
		, p_ALCDevice);

		const ALCchar* name = nullptr;
		ALCCall(if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
			name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
		, p_ALCDevice);
		ALCCall(if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
			name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
		, p_ALCDevice);

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
		ALCCall(if (!alcMakeContextCurrent(nullptr))
			throw std::exception("failed to set context to nullptr");

		alcMakeContextCurrent(nullptr);
		ALCCall(alcDestroyContext(p_ALCContext),p_ALCDevice);
		if (p_ALCContext)
			throw std::exception("failed to unset during close");
		, p_ALCDevice)
		ALCCall(if (!alcCloseDevice(p_ALCDevice))
			throw std::exception("failed to close sound device");
		, p_ALCDevice)
	}
}
}