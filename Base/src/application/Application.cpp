#include "Application.h"

#include "utils/Instrumentor.h"
#include "render/render2D.h"
#include "args/UpdateArgs.h"
#include "render/Colors.h"
#include "GLFW/glfw3.h"
namespace Base
{
	Application::Application(int argc, char** argv)
		:m_ConsoleArgs(argc,argv)
	{
		WindowSpecifications specs;
		specs.Title = m_ConsoleArgs.GetOptionAsString("title");
		specs.Width = m_ConsoleArgs.GetOptionAsInt("width");
		specs.Height = m_ConsoleArgs.GetOptionAsInt("height");
		specs.Fullscreen = m_ConsoleArgs.GetFlag("fullscreen");
		specs.Decorated = !m_ConsoleArgs.GetFlag("title-off");
		specs.VSync_On = !m_ConsoleArgs.GetFlag("vsync-off");
		specs.Resizeble = !m_ConsoleArgs.GetFlag("not-resizeble");

		specs.Title = specs.Title != "" ? specs.Title : "Base";
		specs.Width = specs.Width > -1 ? specs.Width : 1366;
		specs.Height = specs.Height > -1 ? specs.Height : 768;

		WindowProps().width = specs.Width;
		WindowProps().height = specs.Height;

		m_Window = Window::Create(specs);
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_FrameCount = 0;

		Render2D::Init();
		glm::vec3 darker = glm::vec3(Color::Base_Analogous_2.r, Color::Base_Analogous_2.g, Color::Base_Analogous_2.b) * 0.15f;
		Render2D::SetClearColor(glm::vec4(darker,1.0f));
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
			
			//Call render method
			//OnRender();

			//Swap buffer
			m_FrameCount++;
		}
		BASE_TRACE("Game loop Ended! {0} Frames", m_FrameCount);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher disp(e);
		
		disp.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

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