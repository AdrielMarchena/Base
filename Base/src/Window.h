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
			struct ProfileOptions
			{
				char profilePath[50];
				char profileName[25];
				int qtdFrames = 1;
				bool profiling = false;

			} m_ProfileOptions ;
		protected:
			std::string m_Title;
			glm::vec2 m_Resolution = { 800.0f,600.0f };
			float m_AspectRatio = 1.0f;
			bool m_Resizeble;
			float m_Wid;
			float m_Hei;

			bool m_ClampMouse = true;
		public:
			
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

			//Only called on debug
			virtual void OnImGuiDebug() {}

			void SetResizeble(bool resizeble);

			//Initializa the Game Loop
			void Run();

		private:
			void WindowOnImGuiDebug();
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
			void OnWindowResize(const ResizeArgs& args);
			void OnWindowMouseAction(const MouseArgs& args);
			void OnWindowKeyboardAction(const KeyboardArgs& args);
			//Event functions

			virtual void OnResize(const ResizeArgs& args){}
			virtual void OnMouseAction(const MouseArgs& args){}
			virtual void OnKeyboardAction(const KeyboardArgs& args){}
		};
	}
}