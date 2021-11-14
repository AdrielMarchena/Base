#include "ImGuiLayer.h"

#include "Application.h"
#include "utils/base_assert.h"
#include "utils/Instrumentor.h"

#include "imgui.h"
#include "ImGuizmo.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
namespace Base
{
	static bool s_ImGuiInitialized = false;

	static void ProfileMenuItem()
	{
		static int count = 0;
		static int frames_count = 0;
		static bool profiling = false;

		if (ImGui::BeginMenu("Profile"))
		{
			ImGui::InputInt("Frames:", &frames_count);
			if (ImGui::Button("Profile") && !profiling)
			{
				count = frames_count;
				profiling = true;
				const std::string filepath = "Runtime_count_" + std::to_string(count) + ".json";
				BASE_PROFILE_BEGIN_SESSION("Runtime_count", filepath);
			}
			if (profiling)
				ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "Profiling");
			ImGui::EndMenu();
		}
		

		if (profiling)
		{
			if (--count <= 0)
			{
				BASE_PROFILE_END_SESSION();
				profiling = false;
				count = 0;
			}
		}

	}

	ImGuiLayer::ImGuiLayer(const std::string& name)
		:Layer(name)
	{
	}

	void ImGuiLayer::OnAttach()
	{
		if (!s_ImGuiInitialized)
		{
			GLFWwindow* window = (GLFWwindow*)Application::Get().GetWindow().GetNative();

			BASE_CORE_ASSERT(window, "There is no current window set as context");
			//ImGui
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			// Setup Platform/Renderer bindings
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 330");
			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			s_ImGuiInitialized = true;
		}
	}

	void ImGuiLayer::OnUpdate(UpdateArgs args)
	{
		BASE_PROFILE_FUNCTION();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("General"))
			{
				static bool quit_selected = false;
				ImGui::MenuItem("Quit", "", &quit_selected); if (quit_selected) Application::Get().Close();

				std::string lab = "VSync = " + std::string(Application::Get().GetWindow().GetVSync() ? "Enabled" : "Disabled");
				if (ImGui::MenuItem(lab.c_str(), ""))
				{
					Application::Get().GetWindow().SetVSync(!Application::Get().GetWindow().GetVSync());
				}

				ImGui::EndMenu();
			}

#ifdef BASE_PROFILING
			ProfileMenuItem();
#endif
			ImGui::EndMainMenuBar();
		}
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
	}

	void ImGuiLayer::ImGuiInitFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::ImGuiEndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
