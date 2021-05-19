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
		SCROLL
	};
	struct MouseArgs
	{
		MouseArgs() = default;
		MouseAction m_action;
		input::Mouse& mouse;

		double_t Xpos;
		double_t Ypos;
		
		double_t Xoffset;
		double_t Yoffset;
		
		int32_t key;
		int32_t action;
		int32_t mods;

	};
}