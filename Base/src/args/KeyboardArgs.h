#pragma once
#include "input/Keyboard.h"
namespace en
{
	enum class KeyboardAction : uint8_t
	{
		INVALID,
		PRESS,
		RELEASE
	};
	struct KeyboardArgs
	{
		KeyboardArgs() = default;
		KeyboardAction k_action;
		input::Keyboard& keyboard;

		int32_t key;
		int32_t scancode;
		int32_t action;
		int32_t mods;
	};
}