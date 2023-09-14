#include "pch.h"
#include "Components.h"

namespace Base {

	template<typename... T>
	void InitComponent()
	{
		([]()
		{
			T();
		}(), ...);
	}

	template<typename... T>
	void InitComponent(ComponentGroup<T...>)
	{
		InitComponent <T...>();
	}

	void Init::InitComponents()
	{
		InitComponent(AllComponent{});
	}
}