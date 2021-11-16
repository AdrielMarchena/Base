#include "Application.h"

#include "input/Keyboard.h"

#include "utils/Instrumentor.h"
#include "render/render2D.h"
#include "args/UpdateArgs.h"
#include "render/Colors.h"
#include "GLFW/glfw3.h"
namespace Base
{
	Application* Application::m_AppInstance = nullptr;
	Application::Application(int argc, char** argv)
		:m_ConsoleArgs(argc,argv)
	{
		BASE_PROFILE_FUNCTION();

		m_LuaContext.CreateLuaState();

		WindowSpecifications specs;
		if (m_LuaContext.ExecuteFromFile("configurations.lua"))
		{
			specs.Title = m_LuaContext.GetGlobal<std::string>("title");
			specs.Width = m_LuaContext.GetGlobal<int>("width");
			specs.Height = m_LuaContext.GetGlobal<int>("height");
			specs.Fullscreen = m_LuaContext.GetGlobal<bool>("fullscreen");
			specs.Decorated = !m_LuaContext.GetGlobal<bool>("title_off");
			specs.VSync_On = !m_LuaContext.GetGlobal<bool>("vsync_off");
			specs.Resizeble = !m_LuaContext.GetGlobal<bool>("not_resizeble");
		}

		specs.Title = specs.Title != "" ? specs.Title : "Base";
		specs.Width = specs.Width > -1 ? specs.Width : 1366;
		specs.Height = specs.Height > -1 ? specs.Height : 768;

		WindowProps().width = specs.Width;
		WindowProps().height = specs.Height;

		m_AppInstance = this;
		m_Window = Window::Create(specs);
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_FrameCount = 0;

		Render2D::Init();
		glm::vec3 darker = glm::vec3(Color::Base_Analogous_2.r, Color::Base_Analogous_2.g, Color::Base_Analogous_2.b) * 0.128f;
		Render2D::SetClearColor(glm::vec4(darker,1.0f));

		m_ImGuiLayer.OnAttach();
	}

	Application::~Application()
	{
		Render2D::Dispose();
	}

	void Application::Run()
	{
		double deltaTime = 0.0;
		double lastFrame = 0.0;
		BASE_TRACE("Game loop starting!");

		m_Running = true;
		while (m_Running)
		{
			if (input::Keyboard::isPress(BASE_KEY_LEFT_CONTROL) && input::Keyboard::isPress(BASE_KEY_Q))
				Close();
			BASE_PROFILE_SCOPE("Loop");
			//Check Events
			m_Window->OnUpdate();

			// Delta Time and FPS calculations
			double currentTime = glfwGetTime();
			deltaTime = currentTime - lastFrame;
			lastFrame = currentTime;

			//Game Update 
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(deltaTime);
			
			m_ImGuiLayer.ImGuiInitFrame();

			m_ImGuiLayer.OnUpdate(deltaTime);

			m_ImGuiLayer.ImGuiEndFrame();
			m_FrameCount++;
		}
		BASE_TRACE("Game loop Ended! {0} Frames", m_FrameCount);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher disp(e);
		
		disp.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		m_ImGuiLayer.OnEvent(e);
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