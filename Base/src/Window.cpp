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

#include "render/Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

#include "utils/Files.h"

#define CALLBACK_STATIC_CAST(type,window) static_cast<type*>(glfwGetWindowUserPointer(window))

namespace en
{
	namespace windowing
	{
		using namespace utils;

		Window::Window(const char* title, float_t w, float_t h, bool resizeble)
			:m_Title(title), m_Wid(w), m_Hei(h), m_Resizeble(resizeble), m_camera(1, false)
		{
			//Window things
			if (glfwInit() == GLFW_FALSE)
			{
				std::cout << "Error on glfw initialization" << std::endl;
				return;
			}

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

			glfwMakeContextCurrent(m_Window);
			glfwSwapInterval(1);

			if (glewInit() != GLEW_OK)
			{
				glfwTerminate();
				exit(EXIT_FAILURE);
			}

			m_Render = std::make_unique<render::render2D>("shaders/vs.shader", "shaders/fs.shader");

			//Set callback and pointer to this very window
			myWindow = this;
			glfwSetWindowUserPointer(m_Window, myWindow);

			glfwSetWindowSizeCallback(m_Window, on_resize);
			glfwSetScrollCallback(m_Window, on_mouse_scroll);
			glfwSetCursorPosCallback(m_Window, on_cursor_move);
			glfwSetMouseButtonCallback(m_Window, on_mouse_button);
			glfwSetKeyCallback(m_Window, on_keyboard_button);

			//ImGui
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			m_IO = &ImGui::GetIO();
			// Setup Platform/Renderer bindings
			ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
			ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
			// Setup Dear ImGui style
			ImGui::StyleColorsDark();

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
			render2D& render = *m_Render;

			render.GetShader().Bind();
			render.GetShader().SetUniformMat4f("u_ViewProj", m_camera.GetCamera().GetViewProjectionMatrix());
			render.GetShader().SetUniformMat4f("u_Transform", glm::ortho(0.0f, m_Wid, 0.0f, m_Hei, -1.0f, 10.0f));
			render.GetLineShader().Bind();
			render.GetLineShader().SetUniformMat4f("u_ViewProj", m_camera.GetCamera().GetViewProjectionMatrix());
			render.GetLineShader().SetUniformMat4f("u_Transform", glm::ortho(0.0f, m_Wid, 0.0f, m_Hei, -1.0f, 10.0f));
			
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

			float deltaTime = 0.0f;
			float lastFrame = 0.0f;

			OnAttach({ render, m_camera });

			while (!glfwWindowShouldClose(m_Window))
			{
				glfwPollEvents();

				float currentTime = glfwGetTime();
				deltaTime = currentTime - lastFrame;
				lastFrame = currentTime;
				UpdateArgs up_args = { deltaTime,mouse,keyboard,m_pos(mouse) };
				m_camera.OnUpdate(up_args);
				render.GetShader().Bind();
				render.GetShader().SetUniformMat4f(
					"u_ViewProj",
					m_camera.GetCamera().GetViewProjectionMatrix()
				);
				render.GetLineShader().Bind();
				render.GetLineShader().SetUniformMat4f(
					"u_ViewProj",
					m_camera.GetCamera().GetViewProjectionMatrix()
				);
				OnUpdate(up_args);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				render.LineBeginBatch();
				render.BeginBatch();

				OnRender({ deltaTime,render,m_camera,m_camera.GetCamera() });

				render.EndBatch();
				render.LineEndBatch();
				render.Flush();
				render.LineFlush();

				DEAR_NEW_FRAME();

				OnImGui({ render });

				DEAR_END_FRAME();

				glfwSwapBuffers(m_Window);
			}
			Dispose();
		}
		void Window::OnUpdate(UpdateArgs args)
		{
			if (args.keyboard.isPress(GLFW_KEY_LEFT_CONTROL) && args.keyboard.isPress(GLFW_KEY_Q))
				exit(0);

		}

		void Window::OnAttach(AttachArgs args)
		{
		}

		void Window::OnRender(RenderArgs args)
		{
		}

		void Window::OnImGui(ImGuiArgs args)
		{
		}

		void Window::Dispose()
		{
			glfwTerminate();
			try
			{
				aux::DeleteDevices();
			}
			catch (const std::exception& ex)
			{
				std::cout << "Could not delete devices! Error: " << ex.what();
			}
		}

		void Window::SetResizeble(bool resizeble)
		{
			m_Resizeble = resizeble;
			glfwWindowHint(GLFW_RESIZABLE, m_Resizeble ? GLFW_TRUE : GLFW_FALSE);
		}

		void Window::on_resize(GLFWwindow* window, int32_t w, int32_t h)
		{
			Window* ptr = CALLBACK_STATIC_CAST(Window, window);
			ResizeArgs args{};
			args.old_w = ptr->m_Wid;
			args.old_h = ptr->m_Wid;
			args.new_w = w;
			args.new_h = h;
			ptr->OnResize(args);

		}
		void Window::on_mouse_scroll(GLFWwindow* window, double_t xOffSet, double_t yOffSet)
		{
			Window* ptr = CALLBACK_STATIC_CAST(Window, window);
			MouseArgs args
			{
				MouseAction::SCROLL,
				ptr->mouse,
				0.0,0.0,
				xOffSet,
				yOffSet,
				NULL,
				NULL,
				NULL
			};
			ptr->OnMouseAction(args);
		}
		void Window::on_cursor_move(GLFWwindow* window, double_t xPos, double_t yPos)
		{
			Window* ptr = CALLBACK_STATIC_CAST(Window, window);
			MouseArgs args
			{
				MouseAction::MOVE,
				ptr->mouse,
				xPos,
				yPos,
				0.0,0.0,
				NULL,
				NULL,
				NULL
			};
			ptr->OnMouseAction(args);
		}
		void Window::on_mouse_button(GLFWwindow* window, int32_t key, int32_t action, int32_t mods)
		{
			Window* ptr = CALLBACK_STATIC_CAST(Window, window);
			MouseArgs args
			{
				action == GLFW_PRESS ? MouseAction::PRESS : MouseAction::RELEASE,
				ptr->mouse,
				0.0,0.0,0.0,0.0,
				key,
				action,
				mods
			};
			ptr->OnMouseAction(args);
		}
		void Window::on_keyboard_button(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
		{
			Window* ptr = CALLBACK_STATIC_CAST(Window, window);
			KeyboardArgs args
			{
				action == GLFW_PRESS ? KeyboardAction::PRESS : KeyboardAction::RELEASE,
				ptr->keyboard,
				key,
				scancode,
				action,
				mods
			};
			ptr->OnKeyboardAction(args);
		}

		void Window::OnResize(ResizeArgs args)
		{
			//CALLBACK_STATIC_CAST(Window, window)->m_camera.Resize(w, h);
			m_Wid = args.new_w;
			m_Hei = args.new_h;
			m_Render->GetShader().Bind();
			m_Render->GetShader().SetUniformMat4f(
				"u_ViewProj",
				m_camera.GetCamera().GetViewProjectionMatrix()
			);
			m_Render->GetShader().SetUniformMat4f("u_Transform", glm::ortho(0.0f, m_Wid, 0.0f, m_Hei, -1.0f, 10.0f));
			
			m_Render->GetLineShader().Bind();
			m_Render->GetLineShader().SetUniformMat4f(
				"u_ViewProj",
				m_camera.GetCamera().GetViewProjectionMatrix()
			);
			m_Render->GetLineShader().SetUniformMat4f("u_Transform", glm::ortho(0.0f, m_Wid, 0.0f, m_Hei, -1.0f, 10.0f));
			glViewport(0, 0, m_Wid, m_Hei);
		}

		void Window::OnMouseAction(MouseArgs args)
		{
			if (args.m_action != MouseAction::INVALID)
				switch (args.m_action)
				{
				case MouseAction::PRESS:
				case MouseAction::RELEASE:
					mouse.on_mouse_button(this->m_Window, args.key, args.action, args.mods);
					break;
				case MouseAction::MOVE:
					mouse.on_mouse_cursor(this->m_Window, args.Xpos, args.Ypos);
					break;
				case MouseAction::SCROLL:
					mouse.on_mouse_scroll(this->m_Window, args.Xoffset, args.Yoffset);
					m_camera.OnMouseScrolled(args.Yoffset);
					m_Render->GetShader().Bind();
					m_Render->GetShader().SetUniformMat4f("u_ViewProj", m_camera.GetCamera().GetViewProjectionMatrix());
					
					m_Render->GetLineShader().Bind();
					m_Render->GetLineShader().SetUniformMat4f("u_ViewProj", m_camera.GetCamera().GetViewProjectionMatrix());
					break;
				}


		}
		void Window::OnKeyboardAction(KeyboardArgs args)
		{
			if (args.k_action != KeyboardAction::INVALID)
				keyboard.on_keyboard_button(this->m_Window, args.key, args.scancode, args.action, args.mods);
		}


		//Copy things and Bizarre Contructors
		Window::Window(Window&& other) noexcept
			:m_IO(nullptr), m_camera(1)
		{
			//TODO: add mouse, kyeboard, render ,camera here
			m_Window = other.m_Window;
			m_Wid = other.m_Wid;
			m_Hei = other.m_Hei;
			m_Resizeble = other.m_Resizeble;
			m_IO = other.m_IO;
			m_Title = std::move(other.m_Title);
			m_Render = std::move(other.m_Render);
			keyboard = other.keyboard;
			mouse = other.mouse;
			m_camera = other.m_camera;

			//Kill other
			other.m_Window = nullptr;
			m_Title.clear();
			other.m_Wid = NULL;
			other.m_Hei = NULL;
			other.m_Resizeble = NULL;
			other.m_IO = nullptr;

		}

		Window& Window::operator=(Window&& other) noexcept
		{
			if (this == &other)
				return *this;

			//Clear actual instance (usually when heap allocated)
			m_Window = nullptr;
			m_Title.clear();

			//Grab data from other
			m_Window = other.m_Window;
			m_Wid = other.m_Wid;
			m_Hei = other.m_Hei;
			m_Resizeble = other.m_Resizeble;
			m_Title = std::move(other.m_Title);
			m_Render = std::move(other.m_Render);
			keyboard = other.keyboard;
			mouse = other.mouse;
			m_camera = other.m_camera;

			//Kill other
			other.m_Window = nullptr;
			other.m_Wid = 0;
			other.m_Hei = 0;
			other.m_Resizeble = NULL;
			other.m_Title.clear();

			return *this;

		} //Moveble

	}
}
