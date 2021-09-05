/*
* @file Window.h
* 
* @author Adriel Marchena Santos
* Contact: adriel_contato@hotmail.com
*/

#pragma once
#include <memory>
#include <algorithm>
#include <string>
#include "args/VirtualArgs.h" //TODO: Change the args, probably remove
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "glm/glm.hpp"
#include "camera/OrthographicCameraController.h" //TODO: Remove

namespace en
{
	namespace windowing
	{
		using namespace utils;
		class Window
		{
		private:
			Window* myWindow;
			uint64_t nbFrame = 0;
		protected:
			std::string m_Title;
			
			glm::vec2 m_Resolution = { 800.0f,600.0f };
			float m_AspectRatio = 1.0f;
			bool m_Resizeble;

			bool m_ClampMouse = true;

		public:
			//TODO: Make mouse and keyboard Static classes
			input::Mouse mouse;
			input::Keyboard keyboard;
			float m_Wid;
			float m_Hei;

			Window(const char* title = "Base", float_t w = 800, float_t h = 600, bool resizeble = true);
			virtual ~Window();

			Window(Window&) = delete;
			Window& operator=(Window& other) = delete;
			Window& operator=(Window&& other) = delete;

			//TODO: Remove anything inside this functions, should exist a Window_Dispose for example, 
			// so the client don't need to call Window::Dispose() to things work
			
			//Called before the Game Loop start
			virtual void OnAttach();
			//Called on the loop, before OnRender
			virtual void OnUpdate(const UpdateArgs& args);
			//Called on the loop
			virtual void OnRender();
			//Last thing called on the Loop
			virtual void OnImGui();
			//Called after the Game Loop ends
			virtual void Dispose();

			void SetResizeble(bool resizeble);

			//Initializa the Game Loop
			void Run();

		private:
			/*//Used as callback in glfw
			static void on_resize(GLFWwindow* window, int32_t w, int32_t h);
			//Used as callback in glfw
			static void on_mouse_scroll(GLFWwindow* window, double_t xOffSet, double_t yOffSet);
			//Used as callback in glfw
			static void on_cursor_move(GLFWwindow* window, double_t xPos, double_t yPos);
			//Used as callback in glfw
			static void on_mouse_button(GLFWwindow* window, int32_t key, int32_t action, int32_t mods);
			//Used as callback in glfw
			static void on_keyboard_button(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
			//Used as callback in glfw
			static void on_mouse_enter(GLFWwindow* window, int32_t entered);*/

		protected:
			// Return the mouse position with the y axis inverted
			glm::vec2 correct_mouse(input::Mouse& mouse)
			{
				if (m_ClampMouse)
				{
					mouse.clamp_cursor(0.0f, m_Wid, 0.0f, m_Hei);
					ClampTMouse();
				}
				return { mouse.gpos().x, -(mouse.gpos().y - m_Hei) };
			}

			//Return a correted position of the cursor, taking in consideration the camera position
			/*glm::vec2 m_pos(input::Mouse& mouse)
			{
				glm::vec2 mpos = correct_mouse(mouse);
				mpos += glm::vec2(m_camera.GetCamera().GetPosition().x, m_camera.GetCamera().GetPosition().y) * (glm::vec2(m_Wid, m_Hei) / 2.0f);
				return mpos;
			}*/

			inline void ClampTMouse();

			//Hide the cursor on the window
			//Whitout removing the funtionality
			void HideCursor();
			//Unhide the cursor
			void UnhideCursor();

			//Return true if the mouse is hovering the window
			bool CursorHoveredWindow();

			//const inline std::vector<const render::Shader*>& GetVecShaders() { return render_shaders; }
		public:
			//Event functions
			
			//Call super first
			virtual void OnResize(ResizeArgs args);
			//Call super first
			virtual void OnMouseAction(MouseArgs args);
			//Call super first
			virtual void OnKeyboardAction(KeyboardArgs args);
		};
	}
}