#include "LuaContext.h"
#include "utils/base_assert.h"

namespace Base
{
	LuaContext::~LuaContext()
	{
		lua_close(L);
	}

	void LuaContext::CreateLuaState()
	{
		L = luaL_newstate();
		luaL_openlibs(L);
	}

	void LuaContext::Execute(const std::string& script)
	{
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
		int r = luaL_dofile(L, path.c_str());
		if (r == LUA_OK)
			return true;
		std::string errormsg = lua_tostring(L, -1);
		BASE_CORE_ASSERT(false, "Error while executin Lua script '{0}' ERROR: {1}", path, errormsg);
		return false;
	}
}