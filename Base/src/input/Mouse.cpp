/*
*	@file Mouse.cpp
*	@author Adriel Marchena Santos
*	
*	Mouse stuff
*
*	Implement File
*/

#include "Mouse.h"
#include <iostream>
#include "Log.h"
namespace Base
{
namespace input
{

	std::array<bool, EN_MOUSE_NUMBER_KEYS> Mouse::keysPressed{false};
	std::array<bool, EN_MOUSE_NUMBER_KEYS> Mouse::singleClickControl{false};

	glm::vec2 Mouse::pos		= {0.0f,0.0f};
	glm::vec2 Mouse::prevPos	= {0.0f,0.0f};
	glm::vec2 Mouse::OffValue	= {0.0f,0.0f};
	glm::vec2 Mouse::PrevOff	= {0.0f,0.0f};
	bool Mouse::AnyKey = false;

	void Mouse::Init()
	{

	}

	bool Mouse::isPress(int32_t key)
	{
		if (key > 16)
			return false;
		return keysPressed[key];
	}

	bool Mouse::isClicked(int32_t key)
	{
		if (key > 16)
			return false;
		if (!keysPressed[key] || singleClickControl[key])
			return false;

		singleClickControl[key] = true;
		return keysPressed[key];
	}

	bool Mouse::isAnyKey()
	{
		return AnyKey;
	}

	void Mouse::on_mouse_scroll(double_t xOffSet, double_t yOffSet)
	{
		auto n = glm::vec2(xOffSet, yOffSet);
		if (OffValue == n)
			return;
		PrevOff = OffValue;
		OffValue = n;

	}
	void Mouse::on_mouse_button(int32_t key, int32_t action, int32_t mods)
	{
		if (key > 16)
			return;
		switch (action)
		{
		case BASE_PRESS:

			keysPressed[key] = true;
			AnyKey = true;
			break;
		case BASE_BUTTON_RELEASE:
			keysPressed[key] = false;
			singleClickControl[key] = false;
			AnyKey = false;
			break;
		case BASE_REPEAT:
			break;
		default:
			BASE_WARN("Unknow Keyboard Action '{0}'!", action);
			//std::cout << "Unknow Mouse Action '" << action << "'!" << std::endl;
		}
	}
	void Mouse::on_mouse_cursor(double_t xPos, double_t yPos)
	{
		auto n = glm::vec2(xPos, yPos);
		if (pos == n)
			return;
		prevPos = pos;
		pos = n;
	}

	void Mouse::clamp_cursor(float_t min_x, float_t max_x, float_t min_y, float_t max_y)
	{
		pos.x = std::clamp(pos.x, min_x, max_x);
		pos.y = std::clamp(pos.y, min_y, max_y);
	}
}
}