#pragma once
#include "Base/Base.h"
#include "utils/parse_args.h"
#include "event/Events.h"
#include "event/ApplicationEvent.h"
#include "window/Window.h"
#include "Layer.h"
#include "LayerStack.h"
#include <string>
namespace Base
{
	class Application
	{
	private:
		ConsoleArgsParser m_ConsoleArgs;
		Scope<Window> m_Window;
		LayerStack m_LayerStack;
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
	private:
		bool OnWindowClose(WindowCloseEvent& e);
	};

	Application* CreateApplication(int argc, char** argv);
}

