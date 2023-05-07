#pragma once

#include "Base.h"
#include "scene/Scene.h"
#include "scene/Entity.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
namespace Base {
	class PropertiesPanel
	{
	public:
		PropertiesPanel() = default;
		PropertiesPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);
		void SetSelectionContext(Entity selection);
		void OnImGuiRender();



	private:
		void DrawComponents();
		template<typename T> requires Derived<T, Component<T>>
		void DrawTreeComponent(const std::string& name, std::function<void()> job, bool deletable = true)
		{
			static const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
			if (!(m_SelectionContext && m_SelectionContext.HasComponent<T>())) return;

			bool deleted = false;
			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());

			if (deletable)
				deleted = DeletableComponent<T>();

			if (opened)
			{
				job();
				ImGui::TreePop();
			}

			if (deleted)
				m_SelectionContext.RemoveComponent<T>();
		}

		template<typename T> requires Derived<T, Component<T>>
		bool DeletableComponent()
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });

			bool result = false;
			const std::string popupName = Component<T>::ComponentName + " Settings";
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			if (ImGui::Button("+", ImVec2{ 20, 20 }))
			{
				ImGui::OpenPopup(popupName.c_str());
			}
			ImGui::PopStyleVar();
			if (ImGui::BeginPopup(popupName.c_str()))
			{
				if (ImGui::MenuItem("Remove Component"))
					result = true;
				ImGui::EndPopup();
			}
			return result;
		}

		template<typename T> requires Derived<T, Component<T>>
		void AddComponent(std::function<void(T&)> onCreated = nullptr)
		{
			const std::string menu = "Add " + Component<T>::ComponentName;
			if (ImGui::MenuItem(menu.c_str(), 0, false, !m_SelectionContext.HasComponent<T>()))
			{
				auto& component = m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
				if (onCreated)
					onCreated(component);
			}
		}
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
		std::function<void(Entity entity)> m_OnEntitySelected = nullptr;
	};
}
