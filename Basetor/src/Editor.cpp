#include "Editor.h"

#include "scene/Components.h"
#include "render/render2D.h"
#include "utils/Instrumentor.h"
#include "utils/RandomList.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "glm/gtc/type_ptr.hpp"
namespace Base
{

	static void ProfileMenuItem()
	{
#ifdef BASE_PROFILING
		if (ImGui::BeginMenu("Profile"))
		{
			static int count = 0;
			static int frames_count = 0;
			static bool profiling = false;

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

			if (profiling)
			{
				if (--count <= 0)
				{
					BASE_PROFILE_END_SESSION();
					profiling = false;
					count = 0;
				}
			}
			ImGui::EndMenu();
		}
#endif
	}

	Editor::Editor(const std::string& name)
		:Layer(name)
	{
		BASE_PROFILE_FUNCTION();
		Random::Init();
	}

	static inline int IntRandomThing()
	{
		return std::clamp(P_random() * ((Random::Float() - 0.5f)), 0.0f, 1.0f);
	}

	static Entity CreateQuad(Ref<Scene> scene)
	{
		static int count = 0;
		Entity ent = scene->CreateEntity("Quad_" + std::to_string(count));

		ent.AddComponent<Base::SpriteComponent>(Color::Base_Color + (1.0f / count)); //Add sprite (solid color)

		auto& quad_rbody = ent.AddComponent<Base::RigidBody2DComponent>();
		auto& quad_bcol = ent.AddComponent<Base::BoxColider2DComponent>();

		quad_rbody.Type = Base::RigidBody2DComponent::BodyType::Dynamic;

		quad_bcol.Density = IntRandomThing();
		quad_bcol.Friction = IntRandomThing();
		quad_bcol.Restitution = IntRandomThing();
		quad_bcol.RestitutionThreshold = IntRandomThing();

		count++;
		return ent;
	}

	void Editor::OnAttach()
	{
		BASE_PROFILE_FUNCTION();

		m_Scene = MakeRef<Scene>(); //Create scene

		m_Serializer = MakeScope<SceneSerializer>(m_Scene);

		FrameBufferRenderSpecification specs;
		int w = Base::WindowProps().width;
		int h = Base::WindowProps().height;
		specs.width = w;
		specs.height = h;
		specs.scale_factor = 1.0f;
		m_FramebufferRender = MakeScope<FramebufferRender>(specs);

		m_EditorCamera = Base::EditorCamera(45.0f, w / h, 0.01f, 1000.0f);
		m_Scene->OnViewPortResize(w, h);
		m_EditorCamera.SetViewportSize(w, h);

		if (m_Serializer->Deserialize("assets/scenes/scene4.base"))
		{
			auto scene_camera = m_Scene->GetPrimaryCamera();
			if (scene_camera)
				m_Camera = scene_camera;
			else
				m_Camera = m_Scene->CreateEntity("Main2D_Camera"); //Create camera entity
			
			auto& Camera_Script = m_Camera.AddComponent<Base::NativeScriptComponent>();

			if (!m_Camera.HasComponent< CameraComponent>())
			{
				m_Camera.AddComponent<Base::CameraComponent>();
			}

			auto& Camera_comp = m_Camera.GetComponent<Base::CameraComponent>();
			Camera_comp.Camera.SetViewportSize(w, h);

			Camera_Script.Bind<Base::OrthoCameraScript>();
			
			auto& Camera_Transform = m_Camera.GetComponent<Base::TransformComponent>();

			m_Scene->StartNativeScript(m_Camera);
			m_Scene->AwakeNativeScript(m_Camera);
			return;
		}

		{
			m_Entitys["Platform"] = m_Scene->CreateEntity("Platform"); //Create the Quad entity
			m_Entitys["Platform"].AddComponent<Base::SpriteComponent>(Color::Green); //Add sprite (solid color)
			auto& plat_tranform = m_Entitys["Platform"].GetTransform();

			plat_tranform.Translation = { 0.0,-10.5, 0.0f };
			plat_tranform.Scale = { 20.0f, 1.0f, 1.0f };
			plat_tranform.Rotation = { 0.0f, 0.0f, 0.0f };

			auto& quad_rbody = m_Entitys["Platform"].AddComponent<Base::RigidBody2DComponent>();
			auto& quad_bcol = m_Entitys["Platform"].AddComponent<Base::BoxColider2DComponent>();

			quad_rbody.Type = Base::RigidBody2DComponent::BodyType::Static;
		}

		{
			m_Entitys["Platform2"] = m_Scene->CreateEntity("Platform2"); //Create the Quad entity
			m_Entitys["Platform2"].AddComponent<Base::SpriteComponent>(Color::Green); //Add sprite (solid color)
			auto& plat_tranform = m_Entitys["Platform2"].GetTransform();

			plat_tranform.Translation = { 0.0,-10.5, 0.0f };
			plat_tranform.Scale = { 20.0f, 1.0f, 1.0f };
			plat_tranform.Rotation = { 0.0f, 0.0f, 0.0f };

			auto& quad_rbody = m_Entitys["Platform2"].AddComponent<Base::RigidBody2DComponent>();
			auto& quad_bcol = m_Entitys["Platform2"].AddComponent<Base::BoxColider2DComponent>();

			quad_rbody.Type = Base::RigidBody2DComponent::BodyType::Static;
		}

		{
			m_Entitys["Platform3"] = m_Scene->CreateEntity("Platform3"); //Create the Quad entity
			m_Entitys["Platform3"].AddComponent<Base::SpriteComponent>(Color::Green); //Add sprite (solid color)
			auto& plat_tranform = m_Entitys["Platform3"].GetTransform();

			plat_tranform.Translation = { 0.0,-10.5, 0.0f };
			plat_tranform.Scale = { 20.0f, 1.0f, 1.0f };
			plat_tranform.Rotation = { 0.0f, 0.0f, 0.0f };

			auto& quad_rbody = m_Entitys["Platform3"].AddComponent<Base::RigidBody2DComponent>();
			auto& quad_bcol = m_Entitys["Platform3"].AddComponent<Base::BoxColider2DComponent>();

			quad_rbody.Type = Base::RigidBody2DComponent::BodyType::Static;
		}

		{
			m_Entitys["Platform4"] = m_Scene->CreateEntity("Platform4"); //Create the Quad entity
			m_Entitys["Platform4"].AddComponent<Base::SpriteComponent>(Color::Green); //Add sprite (solid color)
			auto& plat_tranform = m_Entitys["Platform4"].GetTransform();

			plat_tranform.Translation = { 0.0,-10.5, 0.0f };
			plat_tranform.Scale = { 20.0f, 1.0f, 1.0f };
			plat_tranform.Rotation = { 0.0f, 0.0f, 0.0f };

			auto& quad_rbody = m_Entitys["Platform4"].AddComponent<Base::RigidBody2DComponent>();
			auto& quad_bcol = m_Entitys["Platform4"].AddComponent<Base::BoxColider2DComponent>();

			quad_rbody.Type = Base::RigidBody2DComponent::BodyType::Static;
		}

		{
			Entity circle = m_Scene->CreateEntity("Circle");
		
			//circle.Add
			auto& circle_comp =	circle.AddComponent<CircleComponent>();
		
			circle_comp.Thickness = 1.0f;

			m_Entitys["Circle"] = circle;
		}

		{
			for (int i = 0; i < 15; i++)
			{
				Entity quad = CreateQuad(m_Scene);
				m_Entitys[quad.GetTag()] = quad;
			}
				
		}
		m_Camera = m_Scene->CreateEntity("Main2D_Camera"); //Create camera entity
		auto& Camera_Transform = m_Camera.GetComponent<Base::TransformComponent>();
		auto& Camera_comp = m_Camera.AddComponent<Base::CameraComponent>();

		Camera_comp.Camera.SetViewportSize(w, h);

		auto& Camera_Script = m_Camera.AddComponent<Base::NativeScriptComponent>();
		Camera_Script.Bind<Base::OrthoCameraScript>();
		m_Scene->StartNativeScript(m_Camera);
		m_Scene->AwakeNativeScript(m_Camera);
	}

	void Editor::OnUpdate(UpdateArgs args)
	{
		BASE_PROFILE_FUNCTION();

		using kb = Base::input::Keyboard;
		using ms = Base::input::Mouse;

		m_FramebufferRender->BindFrameBuffer();
		Render2D::ClearColor();

		if(m_MousePickingEnabled && m_ViewportFocused && m_ViewportHovered)
			m_FramebufferRender->ClearAttachment(1, -1);

		if (m_Runtime)
			m_Scene->OnUpdateRuntime(args);
		else
		{
			m_EditorCamera.OnUpdate(args);
			m_Scene->OnUpdateEditor(args, m_EditorCamera);
		}

		if(input::Mouse::isPress(BASE_MOUSE_BUTTON_LEFT) && input::Keyboard::isPress(BASE_KEY_LEFT_SHIFT))
			if (m_MousePickingEnabled && m_ViewportFocused && m_ViewportHovered)
			{
				auto [mx, my] = ImGui::GetMousePos();
				mx -= m_ViewportBounds[0].x;
				my -= m_ViewportBounds[0].y;
				glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
				my = viewportSize.y - my;
				int mouseX = (int)mx;
				int mouseY = (int)my;

				if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
				{
					int pixel_data = m_FramebufferRender->ReadPixel(1, mouseX, mouseY);
					if (pixel_data > -1)
						m_SelectedEntity = { (entt::entity)pixel_data, m_Scene.get() };
					else
						m_SelectedEntity = {}; //Invalid Entity
						//BASE_INFO("Mouse {0}x{1} | Value {2}", mouseX, mouseY, pixel_data);
				}
			}

		m_FramebufferRender->UnbindFrameBuffer();
	}

	void Editor::OnImGuiRender()
	{
		BASE_PROFILE_FUNCTION();

		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static bool p_open = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				if (ImGui::MenuItem("Serialize")) m_Serializer->Serialize("assets/scenes/scene2.base");

				if (ImGui::BeginMenu("Window"))
				{
					auto& window = Application::Get().GetWindow();
					static bool enable_fullscreen = window.IsFullscreen();
					if (ImGui::Checkbox("Fullscreen", &enable_fullscreen))
						window.SetFullscreen(enable_fullscreen);

					bool enable_vsync = window.GetVSync();
					if (ImGui::Checkbox("VSync", &enable_vsync))
						window.SetVSync(enable_vsync);

					bool enable_titlebar = window.IsTitleBar();
					if (ImGui::Checkbox("Title Bar", &enable_titlebar))
						window.SetTitleBar(enable_titlebar);

					bool enable_resize = window.IsResizeble();
					if (ImGui::Checkbox("Resizeble", &enable_resize))
						window.SetResizeble(enable_resize);

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Framebuffer"))
			{
				static float scalor = 1.0f;
				if (ImGui::SliderFloat("Framebuffer Scalar factor", &scalor, 0.1f, 1.0f))
				{
					auto& spec = m_FramebufferRender->GetSpec();
					spec.scale_factor = scalor;
					m_FramebufferRender->InvalidateFrameBuffer();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Render Status"))
			{
				ImGui::Text("Quads in scene: %d", Render2D::GetQuadStats().DrawCount);
				ImGui::Text("Circle in scene: %d", Render2D::GetCircleStats().DrawCount);
				ImGui::Text("Lines in scene: %d", Render2D::GetLineStats().DrawCount);
			
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Runtime"))
			{
				static bool p_on = false;
				ImGui::Checkbox("Physics", &p_on);
				if(ImGui::Button("Start Runtime"))
				{
					m_Runtime = true;
					m_MousePickingEnabled = false;
					if(p_on)
						m_Scene->RuntimeInit();
					if (m_SyncCameraZoom)
					{
						float dist = m_EditorCamera.GetDistance();
						auto& main_runtime_cam = m_Scene->GetPrimaryCamera();
						if (main_runtime_cam)
						{
							auto& Camera = main_runtime_cam.GetComponent<CameraComponent>();
							float zoom_cam = Camera.Camera.GetOrthographicSize();
							zoom_cam += dist;
							zoom_cam = std::clamp(zoom_cam, 1.0f, 45.0f);
							Camera.Camera.SetOrthographicSize(dist);
						}
					}
				}

				if (ImGui::Button("End Runtime"))
				{
					m_Runtime = false;
					m_MousePickingEnabled = true;
					m_Scene->RuntimeStop();
				}

				ImGui::EndMenu();
			}

			ProfileMenuItem();

			ImGui::EndMenuBar();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer().BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
		
		ImVec2 viewport_size = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewport_size))
		{
			m_ViewportSize = { viewport_size.x,viewport_size.y };
			m_Scene->OnViewPortResize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);

			auto& spec = m_FramebufferRender->GetSpec();
			spec.width = m_ViewportSize.x;
			spec.height = m_ViewportSize.y;
			m_FramebufferRender->InvalidateFrameBuffer();
		}
		uint32_t framebufferTextureId = m_FramebufferRender->GetFramebufferImage();
		ImGui::Image((void*)framebufferTextureId, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		if (m_SelectedEntity && m_GizmoType != -1 && m_MousePickingEnabled)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			// Editor camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = m_SelectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = input::Keyboard::isPress(BASE_KEY_LEFT_CONTROL);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				ImGuizmo::DecomposeMatrixToComponents(&transform[0][0], &translation.x, &rotation.x, &scale.x);

				glm::vec3 deltaRotation = glm::radians(rotation) - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		ImGui::End(); //Viewport
		ImGui::PopStyleVar();

		ImGui::Begin("Quad physics");

		if (m_SelectedEntity)
		{
			auto& ent_rbody = m_SelectedEntity.GetComponent<Base::RigidBody2DComponent>();
			auto& ent_bcol = m_SelectedEntity.GetComponent<Base::BoxColider2DComponent>();

			ImGui::SliderFloat("Friction", &ent_bcol.Friction, 0.0f, 1.0f);
			ImGui::SliderFloat("Density", &ent_bcol.Density, 0.0f, 1.0f);
			ImGui::SliderFloat("Restitution", &ent_bcol.Restitution, 0.0f, 1.0f);
			ImGui::SliderFloat("RestitutionThreshold", &ent_bcol.RestitutionThreshold, 0.0f, 1.0f);
		}

		ImGui::End();

		ImGui::End(); //Dockspace
	}

	void Editor::OnDetach()
	{
		m_Scene->RuntimeStop();
	}

	void Editor::OnEvent(Base::Event& e)
	{
		Base::EventDispatcher disp(e);

		disp.Dispatch<Base::WindowResizeEvent>(BIND_EVENT_FN(Editor::OnWindowResize));
		disp.Dispatch<Base::MouseScrollEvent>(BIND_EVENT_FN(Editor::OnMouseScroll));
		disp.Dispatch<Base::KeyPressedEvent>(BIND_EVENT_FN(Editor::OnKeyboardPressed));
	}

	bool Editor::OnWindowResize(Base::WindowResizeEvent& e)
	{
		return false;
	}

	bool Editor::OnMouseScroll(Base::MouseScrollEvent& e)
	{
		m_EditorCamera.OnMouseScroll(e);
		return false;
	}

	bool Editor::OnKeyboardPressed(Base::KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
			// Gizmos
			case BASE_KEY_Q:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = -1;
				break;
			}
			case BASE_KEY_W:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case BASE_KEY_E:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case BASE_KEY_R:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
		}
		return false;
	}

}
