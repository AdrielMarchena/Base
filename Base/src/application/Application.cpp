#include "pch.h"
#include "Application.h"

#include "input/Keyboard.h"
#include "message/GlobalMessenger.h"

#include "script/LuaContext.h"
#include "utils/Instrumentor.h"
#include "render/render2D.h"
#include "Scripting/ScriptEngine.h"
#include "args/UpdateArgs.h"
#include "render/Colors.h"
#include "GLFW/glfw3.h"

namespace Base {
	Application* Application::m_AppInstance = nullptr;
	UUID s_NotifyFpsCountId = 0;
	Application::Application(int argc, char** argv)
		:m_ConsoleArgs(argc, argv)
	{
		BASE_PROFILE_FUNCTION();

		m_LuaContext = new LuaContext();

		m_LuaContext->CreateLuaState();

		WindowSpecifications specs;
		if (m_LuaContext->ExecuteFromFile("configurations.lua"))
		{
			specs.Title = m_LuaContext->GetGlobal<std::string>("title");
			specs.Width = m_LuaContext->GetGlobal<int>("width");
			specs.Height = m_LuaContext->GetGlobal<int>("height");
			specs.Fullscreen = m_LuaContext->GetGlobal<bool>("fullscreen");
			specs.Decorated = !m_LuaContext->GetGlobal<bool>("title_off");
			specs.VSync_On = !m_LuaContext->GetGlobal<bool>("vsync_off");
			specs.Resizeble = !m_LuaContext->GetGlobal<bool>("not_resizeble");
		}

		specs.Title = specs.Title != "" ? specs.Title : "Base";

		WindowProps().width = specs.Width;
		WindowProps().height = specs.Height;

		m_AppInstance = this;
		m_Window = Window::Create(specs);
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_FrameCount = 0;

		Render2D::Init();
		Render2D::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

		ScriptEngine::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		const std::string title = specs.Title;

		GlobalMessenger::AddChannel("fps_count");
		s_NotifyFpsCountId = GlobalMessenger::ListenChannel("fps_count", [title](Message message)
		{
			std::string titleBar = title;
			static const std::string token = "(%FPS%)";
			auto location = titleBar.find(token);

			titleBar.replace(location, token.length(), message.getEvent());

			m_AppInstance->GetWindow().SetTitlebarText(titleBar);
		});

		if (m_LuaContext)
			delete m_LuaContext;
		m_LuaContext = nullptr;
	}

	Application::~Application()
	{
		if (m_LuaContext)
			delete m_LuaContext;
		Render2D::Dispose();
		ScriptEngine::Shutdown();
	}

	void Application::Run()
	{
		double deltaTime = 0.0;
		double lastFrame = 0.0;
		BASE_TRACE("Game loop starting!");

		double deltaTimeFpsCount = 0.0;
		double lastFrameFpsCount = 0.0;

		double fpsCount = 0.0;

		m_Running = true;
		while (m_Running)
		{
			BASE_PROFILE_SCOPE("Loop");
			//Check Events
			m_Window->OnUpdate();
			GlobalMessenger::NotifyAllChannel();
			
			// Delta Time and FPS calculations
			double currentTime = glfwGetTime();
			deltaTime = currentTime - lastFrame;
			lastFrame = currentTime;
			fpsCount++;

			if (currentTime - lastFrameFpsCount >= 1.0)
			{
				const std::string vsync = fmt::format(" VSync {}", GetWindow().GetVSync() ? "On" : "Off");
				GlobalMessenger::SendMessageC("fps_count", Message{ fmt::format("Fps: {}{}", fpsCount, vsync) });
				lastFrameFpsCount = currentTime;
				fpsCount = 0.0;
			}

			//Game Update 
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(deltaTime);

			m_ImGuiLayer->ImGuiInitFrame();

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			m_ImGuiLayer->ImGuiEndFrame();
			m_FrameCount++;
		}
		BASE_TRACE("Game loop Ended! {0} Frames", m_FrameCount);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher disp(e);

		disp.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		//m_ImGuiLayer->OnEvent(e);
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			if (e.Handled)
				break;
			(*--it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		Close();
		return true;
	}

}