#include "pch.h"
#include "SceneHierarchyPanel.h"

#include <imgui.h>

namespace Base {
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}
	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}
	void SceneHierarchyPanel::SetSelectedEntityCallBack(std::function<void(Entity entity)> onEntitySelected)
	{
		m_OnEntitySelected = onEntitySelected;
	}
	void SceneHierarchyPanel::OnImGuiRender()
	{
		BASE_CORE_ASSERT(m_Context != nullptr, "No context (Scene) provided to the panel (SceneHierarchyPanel)");
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entityId)
		{
			Entity entity{ entityId, m_Context.get() };
			DrawEntityNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectionContext = {};
			if (m_OnEntitySelected != nullptr) // TODO: Check if is the same entity (or leave like this, it's kind of a "rerun" of the callback this way)
			{
				m_OnEntitySelected({});
			}
		}
		ImGui::End();
	}
	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetTag();

		ImGuiTreeNodeFlags flags = (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
			if (m_OnEntitySelected != nullptr) // TODO: Check if is the same entity (or leave like this, it's kind of a "rerun" of the callback this way)
			{
				m_OnEntitySelected(entity);
			}
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}
}