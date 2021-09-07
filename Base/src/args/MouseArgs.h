/*
*	@file MouseArgs.h
*	@author Adriel Marchena Santos
*	
*	Args passed to a Window method or event
*/

#pragma once
#include "input/Mouse.h"
namespace en
{
	enum class MouseAction : uint8_t
	{
		INVALID,
		MOVE,
		PRESS,
		RELEASE,
		SCROLL,
		ENTER,
		LEAVE
	};
	/**
	* Some info about mouse events
	*/
	struct MouseArgs
	{
		MouseArgs() = default;
		MouseAction m_action;

		double_t Xpos;
		double_t Ypos;
		
		double_t Xoffset;
		double_t Yoffset;
		
		int32_t key;
		int32_t action;
		int32_t mods;

	};
}