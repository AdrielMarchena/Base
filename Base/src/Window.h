/*
* @file Window.h
* 
* @author Adriel Marchena Santos
* Contact: adriel_contato@hotmail.com
*/

#pragma once
#include <math.h>
#include <memory>
#include <algorithm>
#include <string>
#include "gl/glew.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "args/VirtualArgs.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "render/SubTexture.h"
#include "utils/Files.h"
#include "utils/Logs.h"
#include "render/render2D.h"
#include "glm/glm.hpp"
#include "camera/OrthographicCameraController.h"
#include "audio/AudioDevice.h"
#include "AL/al.h"
#include "AL/alc.h"
#include <memory>
#include "Log.h"

#define DEAR_NEW_FRAME() ImGui_ImplOpenGL3_NewFrame();\
						 ImGui_ImplGlfw_NewFrame();\
						 ImGui::NewFrame()

#define DEAR_END_FRAME() ImGui::Render();\
						 ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData())

namespace en
{
	namespace windowing
	{
		using namespace utils;
		class Window
		{
		private:
			GLFWwindow* m_Window = nullptr;
			ImGuiIO* m_IO = nullptr;
			input::Mouse mouse;
			input::Keyboard keyboard;
			Window* myWindow;
			OrthographicCameraController m_camera;
			//std::unique_ptr<render::Render2D> m_Render;
			std::vector<const render::Shader*> render_shaders;
			uint64_t nbFrame = 0;
			//render::Render2D m_Render;
		protected:
			std::string m_Title;
			float m_Wid;
			float m_Hei;
			glm::vec2 m_Resolution = { 800.0f,600.0f };
			float m_AspectRatio = 1.0f;
			bool m_Resizeble;

			bool m_ClampMouse = true;

			ALCdevice* p_ALCDevice = nullptr;
			ALCcontext* p_ALCContext = nullptr;


		public:
			
			inline std::shared_ptr<spdlog::logger>& GetLog() { return Log::GetClientLogger(); }
			
			Window(const char* title, float_t w = 800, float_t h = 600, bool resizeble = true);
			virtual ~Window();

			Window(Window&) = delete; //No copies here
			Window(Window&& other) noexcept;
			Window& operator=(Window& other) = delete;
			Window& operator=(Window&& other) noexcept;

			//Called before the Game Loop start
			virtual void OnAttach(AttachArgs args);
			//Called on the loop, before OnRender
			virtual void OnUpdate(UpdateArgs args);
			//Called on the loop
			virtual void OnRender(RenderArgs args);
			//Last thing called on the Loop
			virtual void OnImGui(ImGuiArgs args);
			//Called after the Game Loop ends
			virtual void Dispose();

			void SetResizeble(bool resizeble);

			//Initializa the Game Loop
			void Run();

		private:
			//Used as callback in glfw
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
			static void on_mouse_enter(GLFWwindow* window, int32_t entered);

			inline GLFWwindow* GetWindowOrNot()
			{
				return m_ClampMouse ? m_Window : nullptr;
			}
		protected:
			// Return the mouse position with the y axis inverted

			glm::vec2 correct_mouse(input::Mouse& mouse)
			{
				mouse.clamp_cursor(GetWindowOrNot(),0.0f, m_Wid, 0.0f, m_Hei);
				return { mouse.gpos().x, -(mouse.gpos().y - m_Hei) };
			}

			//Return a correted position of the cursor, taking in consideration the camera position
			glm::vec2 m_pos(input::Mouse& mouse)
			{
				glm::vec2 mpos = correct_mouse(mouse);
				mpos += glm::vec2(m_camera.GetCamera().GetPosition().x, m_camera.GetCamera().GetPosition().y) * (glm::vec2(m_Wid, m_Hei) / 2.0f);
				return mpos;
			}
			void inline SetPerpectiveInShaders();
			void inline SetViewInShaders();
			void inline SetTransformInShaders();

			//Hide the cursor on the window
			//Whitout removing the funtionality
			void HideCursor();
			//Unhide the cursor
			void UnhideCursor();

			//Return true if the mouse is hovering the window
			bool CursorHoveredWindow();

			const inline std::vector<const render::Shader*>& GetVecShaders() { return render_shaders; }
		protected:
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