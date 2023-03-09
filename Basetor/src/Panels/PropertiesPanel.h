#pragma once

#include "Base.h"
#include "scene/Scene.h"
#include "scene/Entity.h"

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
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
		std::function<void(Entity entity)> m_OnEntitySelected = nullptr;
	};
}
