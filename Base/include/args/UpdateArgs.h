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
		float width;
		float height;
		float res_w;
		float res_h;
	};
}