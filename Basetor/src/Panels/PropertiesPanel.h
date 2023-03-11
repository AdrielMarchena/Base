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

		template<typename T>
		void DrawTreeComponent(const std::string& name, std::function<void()> job)
		{
			if (m_SelectionContext && m_SelectionContext.HasComponent<T>())
				if (ImGui::TreeNodeEx((void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, name.c_str()))
				{
					job();
					ImGui::TreePop();
				}
		}
	private:
		void DrawComponents();
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
		std::function<void(Entity entity)> m_OnEntitySelected = nullptr;
	};
}
