#pragma once
#include "lua.hpp"
#include <string>
namespace Base
{
	class LuaContext
	{
	private:
		lua_State* L = nullptr;
	public:
		LuaContext() = default;
		~LuaContext();

		void CreateLuaState();
		lua_State* GetState() { return L; }
		void Execute(const std::string& script);
		bool ExecuteFromFile(const std::string& path);

		//templates here
		template<typename T>
		T GetGlobal(const std::string& variable_name)
		{
		}

		
	};

	template<>
	inline float LuaContext::GetGlobal<float>(const std::string& variable_name)
	{
		lua_getglobal(L, variable_name.c_str());
		if (lua_isnumber(L, -1))
		{
			return (float)lua_tonumber(L, -1);
		}
		return 0.0f;
	}
	
	template<>
	inline int LuaContext::GetGlobal<int>(const std::string& variable_name)
	{
		lua_getglobal(L, variable_name.c_str());
		if (lua_isnumber(L, -1))
		{
			return (int)lua_tonumber(L, -1);
		}
		return 0;
	}
	template<>
	inline bool LuaContext::GetGlobal<bool>(const std::string& variable_name)
	{
		lua_getglobal(L, variable_name.c_str());
		if (lua_isboolean(L, -1))
		{
			return (bool)lua_toboolean(L, -1);
		}
		return false;
	}
	template<>
	inline std::string LuaContext::GetGlobal<std::string>(const std::string& variable_name)
	{
		lua_getglobal(L, variable_name.c_str());
		if (lua_isstring(L, -1))
		{
			return lua_tostring(L, -1);
		}
		return "";
	}

}