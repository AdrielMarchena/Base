#include "Log.h"
#include "utils/base_assert.h"
#include "WindowsWindow.h"
//#include "glad/glad.h"

#include "event/Events.h"
#include "event/ApplicationEvent.h"
#include "event/MouseEvent.h"
#include "event/KeyEvent.h"

#include "input/Mouse.h"
#include "input/Keyboard.h"

#define CALLBACK_STATIC_CAST(type,window) static_cast<type*>(glfwGetWindowUserPointer(window))

namespace Base
{
	static bool s_GLFWInitialized = false;
	static bool s_GLADInitialized = false;

	WindowsWindow::WindowsWindow(const WindowSpecifications& specs)
		:m_Specs(specs)
	{
		Init(specs);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_OpenGLContext->SwapBuffer();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		m_Data.VSync_On = enabled;
		m_Data.VSync_On ? glfwSwapInterval(1) : glfwSwapInterval(0);
	}

	bool WindowsWindow::GetVSync() const
	{
		return m_Data.VSync_On;
	}

	void WindowsWindow::Init(const WindowSpecifications& specs)
	{
		m_Data.Title = specs.Title;
		m_Data.Width = specs.Width;
		m_Data.Height = specs.Height;

		if (!s_GLFWInitialized)
		{
			//Window things
			if (glfwInit() == GLFW_FALSE)
				BASE_CORE_ASSERT(false,"Error on glfw initialization");
			
			BASE_TRACE("GLFW Initialized!");
			s_GLFWInitialized = true;

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_SAMPLES, 4);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

			glfwWindowHint(GLFW_DECORATED, m_Specs.Decorated ? GL_TRUE : GL_FALSE);
			glfwWindowHint(GLFW_RESIZABLE, m_Specs.Resizeble ? GL_TRUE : GL_FALSE);

			s_GLFWInitialized = true;
		}

		int monitors_count;
		GLFWmonitor** monitors = glfwGetMonitors(&monitors_count);
		if (m_Specs.Fullscreen)
		{
			glfwWindowHint(GLFW_DECORATED, GL_FALSE);
			m_Window = glfwCreateWindow(m_Specs.Width, m_Specs.Height, m_Specs.Title.c_str(), monitors[0], nullptr);
			glfwGetMonitorPos(monitors[0],(int*) &m_Data.Width, (int*)&m_Data.Height);
		}
		else
		{
			const GLFWvidmode* vid_mode = glfwGetVideoMode(monitors[0]);
			m_Window = glfwCreateWindow(m_Specs.Width, m_Specs.Height, m_Specs.Title.c_str(), nullptr, nullptr);
			int position_x = (vid_mode->width * 0.5f) - (m_Specs.Width * 0.5f);
			int position_y = (vid_mode->height * 0.5f) - (m_Specs.Height * 0.5f);
			glfwSetWindowPos(m_Window, position_x, position_y);
		}
		BASE_INFO("Window Created, size is {0}x{1}", m_Data.Width, m_Data.Height);

		m_OpenGLContext = MakeScope<GLContext>(m_Window);
		m_OpenGLContext->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(m_Specs.VSync_On);
		BASE_INFO("VSync is {0}!", m_Specs.VSync_On ? "On" : "Off");

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int32_t w, int32_t h)
		{
			auto data = CALLBACK_STATIC_CAST(WindowData, window);
			data->Width = w;
			data->Height = h;
			WindowProps().width = w;
			WindowProps().height = h;
			if (!w && !h)
			{
				WindowProps().minimized = data->Minimized = true;
			}
			WindowResizeEvent evt(w, h);
			data->EventCallback(evt);
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double_t xOffSet, double_t yOffSet)
		{
			auto data = CALLBACK_STATIC_CAST(WindowData, window);
			input::Mouse::on_mouse_scroll(xOffSet, yOffSet);
			MouseScrollEvent evt((float)xOffSet, (float)yOffSet);
			data->EventCallback(evt);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			auto data = CALLBACK_STATIC_CAST(WindowData, window);
			WindowCloseEvent evt;
			data->EventCallback(evt);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double_t xPos, double_t yPos) 
		{
			auto data = CALLBACK_STATIC_CAST(WindowData, window);
			input::Mouse::on_mouse_cursor(xPos, yPos);
			MouseMovedEvent evt(xPos, yPos);
			data->EventCallback(evt);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int32_t key, int32_t action, int32_t mods)
		{
			auto data = CALLBACK_STATIC_CAST(WindowData, window);
			
			input::Mouse::on_mouse_button(key, action, mods);
			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent evt(key);
				data->EventCallback(evt);
				break;
			}
				
			case GLFW_RELEASE:
			{
				MouseButtonReleaseEvent evt(key);
				data->EventCallback(evt);
				break;
			}
			}
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
		{
			auto data = CALLBACK_STATIC_CAST(WindowData, window);
			input::Keyboard::on_keyboard_button(key, scancode, action, mods);
			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent evt(key, 0);
				data->EventCallback(evt);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleaseEvent evt(key);
				data->EventCallback(evt);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent evt(key, 1);
				data->EventCallback(evt);
				break;
			}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t keycode)
			{
				auto data = CALLBACK_STATIC_CAST(WindowData, window);
				//input::Keyboard::on_keyboard_button(keycode, 0, GLFW_PRESS, 0);
				KeyTypedEvent event(keycode);
				data->EventCallback(event);
			});

	}

	void WindowsWindow::Shutdown()
	{
		m_OpenGLContext->Shutdown();
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

}