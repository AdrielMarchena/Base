#pragma once
#include "Base/Base.h"

#include "scene/Entity.h"

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
}
namespace Base {
	class Scene;
	class ScriptClass;
	class ScriptEngine
	{
	public:
		ScriptEngine() = delete;

		static void Init();
		static void Shutdown();

		static void LoadAssembly(const std::filesystem::path& filepath);
		static void LoadAppAssembly(const std::filesystem::path& filepath);

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static bool EntityClassExists(const std::string& fullName);

		static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();

		static void OnCreateEntity(Entity ent);
		static void OnUpdateEntity(Entity ent, float dt);
		static Scene* GetSceneContext();
	private:
		static void InitMono();
		static void ShutdownMono();

		static void LoadAssemblyClasses();
		static MonoObject* InstantiateClass(MonoClass* monoClass);

		static MonoImage* GetCoreAssemblyImage();
		static MonoImage* GetAppAssemblyImage();

		friend class ScriptClass;
		friend class Scene;
		friend class ScriptGlue;
	};

	// ScriptClass

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params, MonoObject** exc);
	private:
		std::string m_ClassNamespace;
		std::string m_Classname;
		MonoClass* m_MonoClass = nullptr;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entityId);

		void InvokeOnCreate();
		void InvokeOnUpdate(float ts);
	private:
		Ref<ScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;
	};
}