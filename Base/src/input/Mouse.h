/*
*	@file Mouse.h
*	@author Adriel Marchena Santos
*	
*	Mouse stuff
*
*/

#pragma once

#include <array>
#include "gl/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <algorithm>

constexpr unsigned char EN_MOUSE_NUMBER_KEYS = 32;
namespace en
{
namespace input
{
	class Mouse
	{
	private:
		std::array<bool, EN_MOUSE_NUMBER_KEYS> keysPressed;
		mutable std::array<bool, EN_MOUSE_NUMBER_KEYS> singleClickControl;

		glm::vec2 pos		= { 0.0f,0.0f };
		glm::vec2 prevPos	= { 0.0f,0.0f };
		mutable glm::vec2 OffValue	= { 0.0f,0.0f };
		glm::vec2 PrevOff	= { 0.0f,0.0f };
		bool AnyKey = false;
	public:
		Mouse()
		: keysPressed(std::array<bool, EN_MOUSE_NUMBER_KEYS>()),
		  singleClickControl(std::array<bool, EN_MOUSE_NUMBER_KEYS>())
		{}

		~Mouse(){}

		bool isClicked(int32_t key)const;
		bool isPress(int32_t key)const;
		
		bool isAnyKey() const;

		void on_mouse_button(GLFWwindow* window, int32_t key, int32_t action, int32_t mods);
		void on_mouse_scroll(GLFWwindow* window, double_t xOffSet, double_t yOffSet);
		void on_mouse_cursor(GLFWwindow* window, double_t xPos, double_t yPos);

		glm::vec2 gpos() const { return pos; }
		glm::vec2 gprevPos() const  { return prevPos; }
		glm::vec2 goffValue() const 
		{ 
			auto aux = OffValue;
			OffValue = { 0.0f,0.0f };
			return aux;
		}
		glm::vec2 gprevOffValue() const { return PrevOff; }

		void clamp_cursor(GLFWwindow* window, float_t min_x, float_t max_x, float_t min_y, float_t max_y);
	};
}
}


