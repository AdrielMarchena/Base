/*
*	@file KeyboardArgs.h
*	@author Adriel Marchena Santos
*	
*	Args passed to a Window method or event
*/

#pragma once
#include "input/Keyboard.h"
namespace Base
{
	enum class KeyboardAction : uint8_t
	{
		INVALID,
		PRESS,
		RELEASE
	};
	/**
	* Some info about keyboard event
	*/
	struct KeyboardArgs
	{
		KeyboardArgs() = default;
		KeyboardAction k_action;

		int32_t key;
		int32_t scancode;
		int32_t action;
		int32_t mods;
	};
}