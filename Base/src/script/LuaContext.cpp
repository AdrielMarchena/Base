#include "pch.h"
#include "LuaContext.h"
#include "utils/base_assert.h"
#include "utils/Instrumentor.h"
namespace Base
{
	LuaContext::~LuaContext()
	{
		BASE_PROFILE_FUNCTION();
		lua_close(L);
	}

	void LuaContext::CreateLuaState()
	{
		BASE_PROFILE_FUNCTION();
		L = luaL_newstate();
		luaL_openlibs(L);
	}

	void LuaContext::Execute(const std::string& script)
	{
		BASE_PROFILE_FUNCTION();
		int r = luaL_dostring(L, script.c_str());

		if (r == LUA_OK)
		{

		}
		else
		{
			std::string errormsg = lua_tostring(L, -1);
			BASE_CORE_ASSERT(false, "Error while executin Lua script '{0}' ERROR: {1}", script, errormsg);
		}
	}

	bool LuaContext::ExecuteFromFile(const std::string& path)
	{
		BASE_PROFILE_FUNCTION();
		int r = luaL_dofile(L, path.c_str());
		if (r == LUA_OK)
			return true;
		std::string errormsg = lua_tostring(L, -1);
		BASE_CORE_ASSERT(false, "Error while executin Lua script '{0}' ERROR: {1}", path, errormsg);
		return false;
	}
}