#pragma once

#include "Base/Base.h"
#include "event/Events.h"
#include "args/UpdateArgs.h"
#include <string>
namespace Base
{
	class Layer
	{
	protected:
		std::string m_DebugName;
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach(){}
		virtual void OnUpdate(UpdateArgs args){}
		virtual void OnImGuiRender(){}
		virtual void OnDetach(){}
		virtual void OnEvent(Event& e){}

		const std::string& GetName() const { return m_DebugName; }
	};
}

