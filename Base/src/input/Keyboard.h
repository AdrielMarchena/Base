/*
*	@file Keyboard.h
*	@author Adriel Marchena Santos
*	
*	Keyboard stuff
*/

#pragma once
#include <array>
#include "gl/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

constexpr unsigned short EN_KEYBOARD_NUMBER_KEYS = 348*2; // Bizarre numbers there
namespace en
{
namespace input
{
	class Keyboard
	{
	private:
		std::array<bool, EN_KEYBOARD_NUMBER_KEYS> keysPressed;
		//TODO: maybe remove this mutable
		mutable std::array<bool, EN_KEYBOARD_NUMBER_KEYS> singleClickControl;

	public:
		Keyboard()
		:keysPressed(std::array<bool, EN_KEYBOARD_NUMBER_KEYS>()),
		 singleClickControl(std::array<bool, EN_KEYBOARD_NUMBER_KEYS>())
		{}

		bool isClicked(int32_t key)const;
		bool isPress(int32_t key)const;

		void on_keyboard_button(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
	};
}
}
