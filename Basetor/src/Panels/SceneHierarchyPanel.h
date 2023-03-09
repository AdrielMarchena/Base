#pragma once

#include "Base.h"
#include "scene/Scene.h"
#include "scene/Entity.h"

namespace Base {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);
		void SetSelectedEntityCallBack(std::function<void(Entity entity)> onEntitySelected);
		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
		std::function<void(Entity entity)> m_OnEntitySelected = nullptr;
	};
}

