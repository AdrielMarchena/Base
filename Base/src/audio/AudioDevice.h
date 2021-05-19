#pragma once
#include "AL/alc.h"

namespace en
{
namespace aux
{
	void LoadDevices();
	void DeleteDevices();

	ALCdevice*  GetDevicePtr();
	ALCcontext* GetContextPtr();

}
}

