/*
* @file Window.h
* 
* @author Adriel Marchena Santos
* Contact: adriel_contato@hotmail.com
*/

#pragma once
#include <math.h>
#include <memory>
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
			std::unique_ptr<render::Render2D> m_Render;
			//render::Render2D m_Render;
		protected:
			std::string m_Title;
			float m_Wid;
			float m_Hei;
			bool m_Resizeble;

			ALCdevice* p_ALCDevice = nullptr;
			ALCcontext* p_ALCContext = nullptr;

		public:
			//Constructors
			Window(const char* title, float_t w = 800, float_t h = 600, bool resizeble = true);
			~Window();

			Window(Window&) = delete; //No copies here
			Window(Window&& other) noexcept;
			Window& operator=(Window& other) = delete;
			Window& operator=(Window&& other) noexcept;

			virtual void OnAttach(AttachArgs args);
			virtual void OnUpdate(UpdateArgs args);
			virtual void OnRender(RenderArgs args);
			virtual void OnImGui(ImGuiArgs args);
			virtual void Dispose();

			void SetResizeble(bool resizeble);

			void Run();

		private:
			static void on_resize(GLFWwindow* window, int32_t w, int32_t h);
			static void on_mouse_scroll(GLFWwindow* window, double_t xOffSet, double_t yOffSet);
			static void on_cursor_move(GLFWwindow* window, double_t xPos, double_t yPos);
			static void on_mouse_button(GLFWwindow* window, int32_t key, int32_t action, int32_t mods);
			static void on_keyboard_button(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);

		protected:
			glm::vec2 correct_mouse(input::Mouse& mouse)
			{
				return { mouse.gpos().x, -(mouse.gpos().y - m_Hei) };
			}

			glm::vec2 m_pos(input::Mouse& mouse)
			{
				glm::vec2 mpos = correct_mouse(mouse);
				return mpos += glm::vec2(m_camera.GetCamera().GetPosition().x, m_camera.GetCamera().GetPosition().y) * (glm::vec2(m_Wid, m_Hei) / 2.0f);
			}
		protected:
			//Event functions
			void OnResize(ResizeArgs args);
			void OnMouseAction(MouseArgs args);
			void OnKeyboardAction(KeyboardArgs args);

		};
	}
}