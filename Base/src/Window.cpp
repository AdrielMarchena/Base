/*
* @file Window.cpp
* 
* 	@author Adriel Marchena Santos
* 	Contact: adriel_contato@hotmail.com
*
*	The Window Class is meant to be the father here
*  	you shold create another class that inheri from Window
*   and use the overrideble methods to make your game, or something
*
*	Implementation File
*/

#include "Window.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "gl/glew.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "render/render2D.h"

#include <iostream>

#include "utils/Files.h"
#include "AL/al.h"
#include "AL/alc.h"
#include "audio/AudioDevice.h"

#include "Log.h"

#define CALLBACK_STATIC_CAST(type,window) static_cast<type*>(glfwGetWindowUserPointer(window))

#define DEAR_NEW_FRAME() ImGui_ImplOpenGL3_NewFrame();\
						 ImGui_ImplGlfw_NewFrame();\
						 ImGui::NewFrame()

#define DEAR_END_FRAME() ImGui::Render();\
						 ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData())

namespace en
{
	namespace windowing
	{

		void on_resize(GLFWwindow* window, int32_t w, int32_t h)
		{
			Window* ptr = CALLBACK_STATIC_CAST(Window, window);
			ResizeArgs args{};
			args.old_w = ptr->m_Wid;
			args.old_h = ptr->m_Hei;
			args.new_w = w;
			args.new_h = h;
			ptr->OnResize(args);

		}
		void on_mouse_scroll(GLFWwindow* window, double_t xOffSet, double_t yOffSet)
		{
			Window* ptr = CALLBACK_STATIC_CAST(Window, window);
			MouseArgs args
			{
				MouseAction::SCROLL,
				0.0,0.0,
				xOffSet,
				yOffSet,
				NULL,
				NULL,
				NULL
			};
			ptr->OnMouseAction(args);
		}
		void on_cursor_move(GLFWwindow* window, double_t xPos, double_t yPos)
		{
			Window* ptr = CALLBACK_STATIC_CAST(Window, window);
			MouseArgs args
			{
				MouseAction::MOVE,
				xPos,
				yPos,
				0.0,0.0,
				NULL,
				NULL,
				NULL
			};
			ptr->OnMouseAction(args);
		}
		void on_mouse_button(GLFWwindow* window, int32_t key, int32_t action, int32_t mods)
		{

			Window* ptr = CALLBACK_STATIC_CAST(Window, window);
			MouseArgs args
			{
				action == GLFW_PRESS ? MouseAction::PRESS : MouseAction::RELEASE,
				0.0,0.0,0.0,0.0,
				key,
				action,
				mods
			};
			ptr->OnMouseAction(args);
		}
		void on_keyboard_button(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
		{
			Window* ptr = CALLBACK_STATIC_CAST(Window, window);
			KeyboardArgs args
			{
				action == GLFW_PRESS ? KeyboardAction::PRESS : KeyboardAction::RELEASE,
				key,
				scancode,
				action,
				mods
			};
			ptr->OnKeyboardAction(args);
		}
		void on_mouse_enter(GLFWwindow* window, int32_t entered)
		{
			Window* ptr = CALLBACK_STATIC_CAST(Window, window);
			MouseArgs args
			{
				MouseAction::LEAVE,
				0.0,0.0,0.0,0.0,
				0,
				0,
				0
			};
			if (entered)
				args.m_action = MouseAction::ENTER;
			ptr->OnMouseAction(args);
		}

		using namespace utils;
		GLFWwindow* m_Window = nullptr;
		ALCdevice* p_ALCDevice = nullptr;
		ALCcontext* p_ALCContext = nullptr;
		ImGuiIO* m_IO = nullptr;

		void Window::ClampTMouse()
		{
			glfwSetCursorPos(m_Window, input::Mouse::gpos().x, input::Mouse::gpos().y);
		}

		void Window::HideCursor()
		{
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			m_ClampMouse = true;
		}

		void Window::UnhideCursor()
		{
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			m_ClampMouse = false;
		}
		
		bool Window::CursorHoveredWindow()
		{
			return glfwGetWindowAttrib(m_Window,GLFW_HOVERED);
		}

		Window::Window(const char* title, float_t w, float_t h, bool resizeble)
			:m_Title(title), m_Wid(w), m_Hei(h), m_Resizeble(resizeble), myWindow(nullptr)
		{

			//Initialize Log system (spdlog)
			Log::Init();

			//Window things
			if (glfwInit() == GLFW_FALSE)
			{
				std::cout << "Error on glfw initialization" << std::endl;
				return;
			}

			BASE_TRACE("GLFW Initialized!");

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_RESIZABLE, resizeble ? GLFW_TRUE : GLFW_FALSE);
			glfwWindowHint(GLFW_SAMPLES, 4);

			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

			m_Window = glfwCreateWindow(m_Wid, m_Hei, title, nullptr, nullptr);

			if (!m_Window)
			{
				glfwTerminate();
				exit(EXIT_FAILURE);
			}

			BASE_TRACE("Window created!");

			glfwMakeContextCurrent(m_Window);
			glfwSwapInterval(0);

			if (glewInit() != GLEW_OK)
			{
				glfwTerminate();
				exit(EXIT_FAILURE);
			}

			BASE_TRACE("Glew Init");

			render::Render2D::Init("shaders/quad_vs.shader",   "shaders/quad_fs.shader",
									"shaders/line_vs.shader",	  "shaders/line_fs.shader",
									"shaders/circle_vs.shader", "shaders/circle_fs.shader",
									"shaders/quad_vs.shader",	  "shaders/text_fs.shader",
									"shaders/quad_vs.shader", "shaders/quad_fs.shader");

			BASE_TRACE("2D Render created!");
			
			//Set callback and pointer to this very window
			myWindow = this;
			glfwSetWindowUserPointer(m_Window, myWindow);

			glfwSetWindowSizeCallback(m_Window, on_resize);
			glfwSetScrollCallback(m_Window, on_mouse_scroll);
			glfwSetCursorPosCallback(m_Window, on_cursor_move);
			glfwSetMouseButtonCallback(m_Window, on_mouse_button);
			glfwSetKeyCallback(m_Window, on_keyboard_button);
			glfwSetCursorEnterCallback(m_Window, on_mouse_enter);

			//ImGui
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			m_IO = &ImGui::GetIO();
			// Setup Platform/Renderer bindings
			ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
			ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
			// Setup Dear ImGui style
			ImGui::StyleColorsDark();

			BASE_TRACE("ImGUI Initialized!");

			//Audio
			aux::LoadDevices();
			p_ALCDevice = aux::GetDevicePtr();
			p_ALCContext = aux::GetContextPtr();
			
		}

		Window::~Window()
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
			if (m_Window)
				glfwDestroyWindow(m_Window);
			glfwTerminate();
		}

		void Window::Run()
		{
			using namespace render;
			using namespace utils;
			auto render = render::Render2D(); //Temp

			SetResizeble(false);

			render.SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

			double deltaTime = 0.0;
			double lastFrame = 0.0;
			double fps = 0.0;
			double fps_lastFrame = 0.0;
			OnAttach();
			
			std::string fps_title = m_Title + " | FPS: " + std::to_string(nbFrame);
			glfwSetWindowTitle(m_Window, fps_title.c_str());

			BASE_TRACE("Game loop starting!");
			
			while (!glfwWindowShouldClose(m_Window))
			{
				//Check Events
				glfwPollEvents();

				// Delta Time and FPS calculations
				double currentTime = glfwGetTime();
				deltaTime = currentTime - lastFrame;
				lastFrame = currentTime;
				nbFrame++;
				
				if (currentTime - fps_lastFrame >= 1.0)
				{
					//fps = 1000.0 / deltaTime;
					UpdateFpsTitle(nbFrame);
					nbFrame = 0;
					fps_lastFrame = currentTime;
				}
				
				//Game Update 
				OnUpdate({ (float)deltaTime,m_Wid,m_Hei,m_Resolution.x,m_Resolution.y });
				UpdateWindow();
				//Call render method
				OnRender();

				//ImGui things
				DEAR_NEW_FRAME();
				OnImGui();
				DEAR_END_FRAME();

				//Swap buffer
				glfwSwapBuffers(m_Window);
			}
			BASE_TRACE("Game loop Ended!");
			Dispose();
			CloseWindow();
		}
		void Window::CloseWindow()
		{
			render::Render2D::Dispose();
			try
			{
				aux::DeleteDevices();
			}
			catch (const std::exception& ex)
			{
				BASE_ERROR(ex.what());
			}
			BASE_TRACE("Dispose completed!");
		}

		void Window::UpdateFpsTitle(double fps)
		{
			//Set FPS on window title
			static std::string fps_title;
			fps_title = m_Title + " | FPS: " + std::to_string(fps);
			glfwSetWindowTitle(m_Window, fps_title.c_str());
		}
		
		void Window::UpdateWindow()
		{
			if (input::Keyboard::isPress(BASE_KEY_LEFT_CONTROL) && input::Keyboard::isPress(BASE_KEY_Q))
				glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
		}

		void Window::SetResizeble(bool resizeble)
		{
			m_Resizeble = resizeble;
			glfwWindowHint(GLFW_RESIZABLE, m_Resizeble ? GLFW_TRUE : GLFW_FALSE);
		}

		void Window::OnResize(ResizeArgs args)
		{
			m_Wid = args.new_w;
			m_Hei = args.new_h;
			m_AspectRatio = m_Wid / m_Hei;
			
			glViewport(0, 0, m_Wid, m_Hei);
		}

		void Window::OnMouseAction(MouseArgs args)
		{
			if (args.m_action != MouseAction::INVALID)
				switch (args.m_action)
				{
				case MouseAction::PRESS:
				case MouseAction::RELEASE:
					input::Mouse::on_mouse_button( args.key, args.action, args.mods);
					break;
				case MouseAction::MOVE:
					input::Mouse::on_mouse_cursor(args.Xpos, args.Ypos);
					break;
				case MouseAction::SCROLL:
					input::Mouse::on_mouse_scroll(args.Xoffset, args.Yoffset);

					break;
				}
		}
		void Window::OnKeyboardAction(KeyboardArgs args)
		{
			if (args.k_action != KeyboardAction::INVALID)
				input::Keyboard::on_keyboard_button(args.key, args.scancode, args.action, args.mods);
		}
	}
}
