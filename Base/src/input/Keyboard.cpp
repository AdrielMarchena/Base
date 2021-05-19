#include "Keyboard.h"
#include <iostream>
namespace en
{
namespace input
{
	bool Keyboard::isPress(int32_t key) const
	{
		key -= 32;
		return keysPressed[key];
	}
	bool Keyboard::isClicked(int32_t key) const
	{
		key -= 32;
		if (!keysPressed[key] || singleClickControl[key])
			return false;
		
		singleClickControl[key] = true;
		return keysPressed[key];
	}
	void Keyboard::on_keyboard_button(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
	{
		key -= 32;
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
			std::cout << "Unknow Keyboard Action '" << action << "'!" << std::endl;
		}
	}
}
}