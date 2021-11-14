#pragma once
#include "Base/Base.h"
#include "window/Window.h"

#include "GLFW/glfw3.h"
#include "render/gl/GLContext.h"
namespace Base
{
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
			uint32_t Width;
			uint32_t Height;
			bool VSync_On;
			bool Minimized;
			EventCallbackFn EventCallback;
		} m_Data;

	public:
		WindowsWindow(const WindowSpecifications& specs);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		uint32_t GetWidth() const override { return m_Data.Width; }
		uint32_t GetHeight() const override { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool GetVSync() const override;
		bool IsMinimized() const { return m_Data.Minimized; }
		void* GetNative() const override { return (void*)m_Window; }
	private:
		virtual void Init(const WindowSpecifications& specs);
		virtual void Shutdown();
	};
}

