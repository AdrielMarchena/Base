#pragma once

#include "Layer.h"

namespace Base
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(const std::string& name = "ImGuiLayer");

		virtual void OnAttach() override;
		virtual void OnUpdate(UpdateArgs args) override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void ImGuiInitFrame();
		void ImGuiEndFrame();
	};
}

