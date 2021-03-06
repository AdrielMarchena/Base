#pragma once
#include "Base/Base.h"
#include "Base.h"
#include "scene/EditorCamera.h"
#include "render/gl/FramebufferRender.h"
#include "scene/SceneSerializer.h"

#include <string>
#include <unordered_map>

namespace Base
{
	class Editor : public Base::Layer
	{
	private:
		Ref<Scene> m_Scene;
		Scope<SceneSerializer> m_Serializer;
		EditorCamera m_EditorCamera;
		Entity m_Camera;
		Entity m_SelectedEntity;
		Scope<FramebufferRender> m_FramebufferRender;
		std::unordered_map<std::string, Base::Entity> m_Entitys;
		bool m_Runtime = false;
		bool m_ViewportFocused = false;
		bool m_ViewportHovered= false;
		bool m_MousePickingEnabled = true;
		bool m_SyncCameraZoom = true;
		int m_GizmoType = -1;


		glm::vec2 m_ViewportSize{1366,768};
		glm::vec2 m_ViewportBounds[2];
	public:
		Editor(const std::string& name = "Editor");

		void OnAttach() override;
		void OnUpdate(UpdateArgs args) override;
		void OnImGuiRender() override;
		void OnDetach() override;
		void OnEvent(Base::Event& e) override;

		bool OnWindowResize(Base::WindowResizeEvent& e);
		bool OnMouseScroll(Base::MouseScrollEvent& e);
		bool OnKeyboardPressed(Base::KeyPressedEvent& e);
	};
}
