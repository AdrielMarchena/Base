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

namespace Base
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
			float m_Wid;
			float m_Hei;

			Window(const char* title = "Base", float_t w = 800, float_t h = 600, bool resizeble = true);
			virtual ~Window();

			Window(Window&) = delete;
			Window(Window&&) = delete;
			Window& operator=(Window& other) = delete;
			Window& operator=(Window&& other) = delete;

			//Called before the Game Loop start
			virtual void OnAttach(){}
			//Called on the loop, before OnRender
			virtual void OnUpdate(const UpdateArgs& args){}
			//Called on the loop
			virtual void OnRender(){}
			//Last thing called on the Loop
			virtual void OnImGui(){}
			//Called after the Game Loop ends
			virtual void Dispose(){}

			void SetResizeble(bool resizeble);

			//Initializa the Game Loop
			void Run();

		private:
			inline void CloseWindow();
			inline void UpdateWindow();
			inline void UpdateFpsTitle(double fps);
		protected:

			//Return a correted position of the cursor, taking in consideration the camera position
			/*glm::vec2 m_pos(input::Mouse& mouse)
			{
				glm::vec2 mpos = correct_mouse(mouse);
				mpos += glm::vec2(m_camera.GetCamera().GetPosition().x, m_camera.GetCamera().GetPosition().y) * (glm::vec2(m_Wid, m_Hei) / 2.0f);
				return mpos;
			}*/
			inline void ClampTMouse();

			void HideCursor();
			void UnhideCursor();

			//Return true if the mouse is hovering the window
			bool CursorHoveredWindow();

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