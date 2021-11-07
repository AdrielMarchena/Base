/*
*	@file UpdateArgs.h
*	@author Adriel Marchena Santos
*	
*	Args passed to a Window method
*/

#pragma once

#include "input/Mouse.h"
#include "input/Keyboard.h"

namespace Base
{
	struct UpdateArgs
	{
		float dt;
	};
}