#include "pch.h"
#include "ScriptableEntity.h"
#include "meta/factory.hpp"
#include "meta/meta.hpp"
namespace Base
{
	ScriptableEntity::ScriptableEntity()
	{
		static bool once = []() 
		{
			std::hash<std::string_view> hash{};
			auto factory = meta::reflect<ScriptableEntity>(hash("ScriptableEntity"));
			return true;
		}();
	}
}