/*
*	@file Keyboard.cpp
*	@author Adriel Marchena Santos
*	
*	Keyboard stuff
*/

#include "Keyboard.h"
#include <iostream>
#include "Log.h"

namespace en
{
namespace input
{
	bool Keyboard::isPress(int32_t key) const
	{
		key -= 32;
		return keysPressed[key];
	}
	bool Keyboard::isClicked(int32_t key) const
	{
		key -= 32;
		if (!keysPressed[key] || singleClickControl[key])
			return false;
		
		singleClickControl[key] = true;
		return keysPressed[key];
	}

	bool Keyboard::isAnyKey() const
	{
		return AnyKey;
	}

	void Keyboard::on_keyboard_button(int32_t key, int32_t scancode, int32_t action, int32_t mods)
	{
		key -= 32;
		switch (action)
		{
		case BASE_PRESS:
			keysPressed[key] = true;
			AnyKey = true;
			break;
		case BASE_RELEASE:
			keysPressed[key] = false;
			singleClickControl[key] = false;
			AnyKey = false;
			break;
		case BASE_REPEAT:
			break;
		default:
			BASE_WARN("Unknow Keyboard Action '{0}'!", action);
			//std::cout << "Unknow Keyboard Action '" << action << "'!" << std::endl;
		}
	}
}
}