/*
*	@file AudioDevice.h
*	@author Adriel Marchena Santos
*	
*	Function definition to load and delete audio devices on the program
*/

#pragma once
#include "AL/alc.h"

namespace Base
{
namespace aux
{
	/**
	* Load audio devices on the PC
	*/
	void LoadDevices();
	/**
	* Delete the devices from the program
	*/
	void DeleteDevices();
	

	ALCdevice*  GetDevicePtr();
	ALCcontext* GetContextPtr();

}
}

