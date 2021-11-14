#pragma once
#include "Base/Base.h"
#include "utils/parse_args.h"
#include "event/Events.h"
#include "event/ApplicationEvent.h"
#include "window/Window.h"
#include "Layer.h"
#include "LayerStack.h"
#include "ImGuiLayer.h"
#include <string>

int main(int argc, char** argv);

namespace Base
{
	class Application
	{
	private:
		ConsoleArgsParser m_ConsoleArgs;
		Scope<Window> m_Window;
		LayerStack m_LayerStack;
		ImGuiLayer m_ImGuiLayer;
		bool m_Running = false;
		uint64_t m_FrameCount;
	public:
		Application(int argc, char** argv);
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		const ConsoleArgsParser& GetCommandLineArguments() const { return m_ConsoleArgs; }
		void Close();

		Window& GetWindow() { return *m_Window; }
		static Application& Get() { return *m_AppInstance; }
		ImGuiLayer& GetImGuiLayer() { return m_ImGuiLayer; }
	private:
		static Application* m_AppInstance;
		bool OnWindowClose(WindowCloseEvent& e);
		friend int ::main(int argc, char** argv); //This is crazy, not even know this was a thing
	};

	Application* CreateApplication(int argc, char** argv);
}

