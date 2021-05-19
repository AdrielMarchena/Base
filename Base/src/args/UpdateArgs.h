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