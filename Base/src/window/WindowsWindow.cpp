#include "pch.h"
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
#include "utils/Instrumentor.h"
#define CALLBACK_STATIC_CAST(type,window) static_cast<type*>(glfwGetWindowUserPointer(window))

namespace Base {
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

	bool WindowsWindow::GetVSync() const
	{
		return m_Data.VSync_On;
	}

	void WindowsWindow::SetTitlebarText(const std::string& title)
	{
		m_Data.Title = title;
		glfwSetWindowTitle(m_Window, title.c_str());
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		m_Data.VSync_On = enabled;
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
	}

	void WindowsWindow::SetFullscreen(bool enabled)
	{
		if (enabled && !glfwIsFullScreen())
		{
			glfwGetWindowPos(m_Window, &m_Data.XPos, &m_Data.YPos);
			glfwGetWindowSize(m_Window, &m_Data.Back_Width, &m_Data.Back_Height);

			const GLFWvidmode* mode = glfwGetVideoMode(m_Monitors[m_CurrentMonitorIndice]);

			glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GL_FALSE);
			glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, GL_FALSE);
			glfwSetWindowMonitor(m_Window, m_Monitors[m_CurrentMonitorIndice], 0, 0, mode->width, mode->height, 0);
		}
		else
		{
			glfwSetWindowMonitor(m_Window, nullptr, m_Data.XPos, m_Data.YPos, m_Data.Back_Width, m_Data.Back_Height, 0);
			SetTitleBar(IsTitleBar());
			SetResizeble(IsResizeble());
		}

		m_Data.Fullscreen = enabled;
	}

	bool WindowsWindow::glfwIsFullScreen() const
	{
		return glfwGetWindowMonitor(m_Window) != nullptr;
	}

	bool WindowsWindow::IsFullscreen() const
	{
		return m_Data.Fullscreen;
	}

	bool WindowsWindow::IsTitleBar() const
	{
		return m_Data.TitleBar;
	}

	bool WindowsWindow::IsResizeble() const
	{
		return m_Data.Resizeble;
	}

	void WindowsWindow::SetTitleBar(bool enabled)
	{
		m_Data.TitleBar = enabled;
		if (enabled)
			glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GL_TRUE);
		else
			glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GL_FALSE);
	}

	void WindowsWindow::SetResizeble(bool enabled)
	{
		m_Data.Resizeble = enabled;
		if (enabled)
			glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, GL_TRUE);
		else
			glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, GL_FALSE);
	}

	void WindowsWindow::Init(const WindowSpecifications& specs)
	{
		BASE_PROFILE_FUNCTION();

		m_Data.Title = specs.Title;
		m_Data.Width = specs.Width;
		m_Data.Height = specs.Height;
		m_Data.Fullscreen = specs.Fullscreen;
		m_Data.Resizeble = specs.Resizeble;
		m_Data.TitleBar = specs.Decorated;

		if (!s_GLFWInitialized)
		{
			//Window things
			if (glfwInit() == GLFW_FALSE)
				BASE_CORE_ASSERT(false, "Error on glfw initialization");

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

		m_Monitors = glfwGetMonitors(&m_MonitorCount);
		m_CurrentMonitorIndice = std::clamp(m_CurrentMonitorIndice, 0, m_MonitorCount);
		if (m_Specs.Fullscreen)
		{
			glfwWindowHint(GLFW_DECORATED, GL_FALSE);
			m_Window = glfwCreateWindow(m_Specs.Width, m_Specs.Height, m_Specs.Title.c_str(), m_Monitors[m_CurrentMonitorIndice], nullptr);
		}
		else
		{
			const GLFWvidmode* vid_mode = glfwGetVideoMode(m_Monitors[m_CurrentMonitorIndice]);
			m_Window = glfwCreateWindow(m_Specs.Width, m_Specs.Height, m_Specs.Title.c_str(), nullptr, nullptr);
			m_Data.XPos = (vid_mode->width * 0.5f) - (m_Specs.Width * 0.5f);
			m_Data.YPos = (vid_mode->height * 0.5f) - (m_Specs.Height * 0.5f);
			glfwSetWindowPos(m_Window, m_Data.XPos, m_Data.YPos);
		}
		if (!m_Specs.Fullscreen)
			BASE_TRACE("Window Created, size is {0}x{1}", m_Data.Width, m_Data.Height);
		else
			BASE_TRACE("Window Created, in fullscreen", m_Data.Width, m_Data.Height);

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
		BASE_PROFILE_FUNCTION();

		m_OpenGLContext->Shutdown();
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

}