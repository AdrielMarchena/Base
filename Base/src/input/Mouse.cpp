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
namespace en
{
namespace input
{
	bool Mouse::isPress(int32_t key) const
	{
		if (key > 16)
			return false;
		return keysPressed[key];
	}

	bool Mouse::isClicked(int32_t key) const
	{
		if (key > 16)
			return false;
		if (!keysPressed[key] || singleClickControl[key])
			return false;

		singleClickControl[key] = true;
		return keysPressed[key];
	}

	bool Mouse::isAnyKey() const
	{
		return AnyKey;
	}

	void Mouse::on_mouse_scroll(GLFWwindow* window, double_t xOffSet, double_t yOffSet)
	{
		auto n = glm::vec2(xOffSet, yOffSet);
		if (OffValue == n)
			return;
		PrevOff = OffValue;
		OffValue = n;

	}
	void Mouse::on_mouse_button(GLFWwindow* window, int32_t key, int32_t action, int32_t mods)
	{
		if (key > 16)
			return;
		switch (action)
		{
		case GLFW_PRESS:

			keysPressed[key] = true;
			AnyKey = true;
			break;
		case GLFW_RELEASE:
			keysPressed[key] = false;
			singleClickControl[key] = false;
			AnyKey = false;
			break;
		case GLFW_REPEAT:
			break;
		default:
			BASE_WARN("Unknow Keyboard Action '{0}'!", action);
			//std::cout << "Unknow Mouse Action '" << action << "'!" << std::endl;
		}
	}
	void Mouse::on_mouse_cursor(GLFWwindow* window, double_t xPos, double_t yPos)
	{
		auto n = glm::vec2(xPos, yPos);
		if (pos == n)
			return;
		prevPos = pos;
		pos = n;
	}

	void Mouse::clamp_cursor(GLFWwindow* window,float_t min_x, float_t max_x, float_t min_y, float_t max_y)
	{
		pos.x = std::clamp(pos.x, min_x, max_x);
		pos.y = std::clamp(pos.y, min_y, max_y);
		if (window)
			glfwSetCursorPos(window, pos.x, pos.y);
	}
}
}