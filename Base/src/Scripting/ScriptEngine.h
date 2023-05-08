#pragma once
#include "Base/Base.h"
namespace Base {
	class ScriptEngine
	{
	private:
		static void InitMono();
		static void ShutdownMono();
	public:
		ScriptEngine() = delete;

		static void Init();
		static void Shutdown();
	};
}