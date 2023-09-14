#pragma once
#include "Base/Base.h"
#include "window/Window.h"

#include "GLFW/glfw3.h"
#include "render/gl/GLContext.h"
namespace Base {
	class WindowsWindow : public Window
	{
	private:
		GLFWwindow* m_Window;
		WindowSpecifications m_Specs;
		Scope<GLContext> m_OpenGLContext;
		bool m_Minimized = false;
		struct WindowData
		{
			std::string Title;
			int32_t Width;
			int32_t Height;
			int32_t Back_Width;
			int32_t Back_Height;
			int32_t XPos = 0;
			int32_t YPos = 0;
			bool VSync_On;
			bool Minimized;
			bool Fullscreen;
			bool TitleBar;
			bool Resizeble;
			EventCallbackFn EventCallback;
		} m_Data;


		int m_MonitorCount;
		GLFWmonitor** m_Monitors;
		int m_CurrentMonitorIndice = 0;
	public:
		WindowsWindow(const WindowSpecifications& specs);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		int32_t GetWidth() const override { return m_Data.Width; }
		int32_t GetHeight() const override { return m_Data.Height; }
		std::string GetTitlebar() const override { return m_Data.Title; }
		void SetTitlebarText(const std::string& title) override;

		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool GetVSync() const override;
		bool IsMinimized() const { return m_Data.Minimized; }
		void* GetNative() const override { return (void*)m_Window; }

		virtual void SetFullscreen(bool enabled) override;
		virtual void SetTitleBar(bool enabled) override;
		virtual void SetResizeble(bool enabled) override;

		bool glfwIsFullScreen() const;
		virtual bool IsFullscreen() const override;
		virtual bool IsTitleBar() const override;
		virtual bool IsResizeble() const override;
	private:
		virtual void Init(const WindowSpecifications& specs);
		virtual void Shutdown();
	};
}

