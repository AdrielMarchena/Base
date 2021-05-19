#include "Mouse.h"
#include <iostream>
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
			break;
		case GLFW_RELEASE:
			keysPressed[key] = false;
			singleClickControl[key] = false;
			break;
		case GLFW_REPEAT:
			break;
		default:
			std::cout << "Unknow Mouse Action '" << action << "'!" << std::endl;
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
}
}