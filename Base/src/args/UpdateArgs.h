/*
*	@file UpdateArgs.h
*	@author Adriel Marchena Santos
*	
*	Args passed to a Window method
*/

#pragma once

#include "input/Mouse.h"
#include "input/Keyboard.h"

namespace en
{
	struct UpdateArgs
	{
		float dt;
		input::Mouse& mouse;
		input::Keyboard& keyboard;
	};
}