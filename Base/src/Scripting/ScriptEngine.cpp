#include "pch.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"

#include "scene/Scene.h"
#include <glm/glm.hpp>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Base {
	namespace Utils {
		static char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			BASE_CORE_ASSERT(stream, "Could not open file '{0}'", filepath.string().c_str());

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = end - stream.tellg();

			BASE_CORE_ASSERT(size > 0, "The file '{0}' is empty", filepath.string().c_str());

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = size;
			return buffer;
		}

		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = ReadBytes(assemblyPath, &fileSize);

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			BASE_CORE_ASSERT(status == MONO_IMAGE_OK, "Error on MonoImage, '{0}'", mono_image_strerror(status));
			if (status != MONO_IMAGE_OK) return nullptr;

			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.string().c_str(), &status, 0);
			mono_image_close(image);

			// Don't forget to free the file data
			delete[] fileData;

			return assembly;
		}
	}
	void RunExample();

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		ScriptClass* DemoClass;

		Scene* SceneContext;

		ScriptClass EntityClass;
		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
		std::unordered_map<UUID, Ref<ScriptInstance>> EntityInstances;
	};
	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();
		InitMono();
		LoadAssembly("Resources/Scripts/Base-ScriptCore.dll");
		LoadAssemblyClasses(s_Data->CoreAssembly);

		ScriptGlue::RegisterFunctions();

		RunExample();
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_Data;
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppDomain = mono_domain_create_appdomain((char*)"BaseDomain", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath);
		BASE_CORE_ASSERT(s_Data->CoreAssembly != nullptr, "Could not load '{0}'", filepath.string().c_str());

		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		// PrintAssemblyTypes(s_Data->CoreAssembly);
	}

	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		s_Data->SceneContext = scene;
	}

	void ScriptEngine::OnRuntimeStop()
	{
		s_Data->SceneContext = nullptr;

		s_Data->EntityInstances.clear();
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullName)
	{
		return s_Data->EntityClasses.find(fullName) != s_Data->EntityClasses.end();
	}

	std::unordered_map<std::string, Ref<ScriptClass>> ScriptEngine::GetEntityClasses()
	{
		return s_Data->EntityClasses;
	}

	void ScriptEngine::OnCreateEntity(Entity ent)
	{
		const auto& sc = ent.GetComponent<ScriptComponent>();
		if (ScriptEngine::EntityClassExists(sc.Name))
		{
			Ref<ScriptInstance> instance = MakeRef<ScriptInstance>(s_Data->EntityClasses[sc.Name], ent);
			s_Data->EntityInstances[ent.GetID()] = instance;
			instance->InvokeOnCreate();
		}
	}

	void ScriptEngine::OnUpdateEntity(Entity ent, float dt)
	{
		BASE_CORE_ASSERT(s_Data->EntityInstances.find(ent.GetID()) != s_Data->EntityInstances.end(), "There is no instance of a script for this entity: {0}", ent.GetID());

		const auto& sc = ent.GetComponent<ScriptComponent>();
		s_Data->EntityInstances[ent.GetID()]->InvokeOnUpdate(dt);
	}

	Scene* ScriptEngine::GetSceneContext()
	{
		return s_Data->SceneContext;
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("BaseJITRuntime");
		BASE_CORE_ASSERT(rootDomain, "Could not initialize Mono");

		s_Data->RootDomain = rootDomain;
	}

	void ScriptEngine::ShutdownMono()
	{
		if (s_Data->DemoClass != nullptr)
			delete s_Data->DemoClass;
		// mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
		// mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;
	}

	void ScriptEngine::LoadAssemblyClasses(MonoAssembly* assembly)
	{
		s_Data->EntityClasses.clear();

		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		MonoClass* monoClass = mono_class_from_name(s_Data->CoreAssemblyImage, "Base", "Entity");
		s_Data->EntityClass = ScriptClass("Base", "Entity");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
			MonoClass* entityClass = mono_class_from_name(s_Data->CoreAssemblyImage, nameSpace, name);

			if (entityClass == monoClass)
				continue;

			std::string fullName;
			if (strlen(nameSpace) != 0)
			{
				fullName = fmt::format("{}.{}", nameSpace, name);
			}
			else
			{
				fullName = name;
			}

			bool isEntity = mono_class_is_subclass_of(entityClass, monoClass, false);
			if (isEntity)
			{
				s_Data->EntityClasses[fullName] = MakeRef<ScriptClass>(nameSpace, name);
			}

			BASE_TRACE(fullName);
		}
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}

	// Script Class

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className)
		: m_ClassNamespace(classNamespace), m_Classname(className)
	{
		m_MonoClass = mono_class_from_name(s_Data->CoreAssemblyImage, classNamespace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount)
	{
		return mono_class_get_method_from_name(m_MonoClass, name.c_str(), parameterCount);
	}
	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params, MonoObject** exc)
	{
		return mono_runtime_invoke(method, instance, params, exc);
	}

	// Script Instance

	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity)
		:m_ScriptClass(scriptClass)
	{
		m_Instance = scriptClass->Instantiate();
		m_Constructor = s_Data->EntityClass.GetMethod(".ctor", 1);
		m_OnCreateMethod = scriptClass->GetMethod("OnCreate", 0);
		m_OnUpdateMethod = scriptClass->GetMethod("OnUpdate", 1);

		UUID entityId = entity.GetID();
		void* param = &entityId;
		m_ScriptClass->InvokeMethod(m_Instance, m_Constructor, &param, nullptr);
	}

	void ScriptInstance::InvokeOnCreate()
	{
		m_ScriptClass->InvokeMethod(m_Instance, m_OnCreateMethod, nullptr, nullptr);
	}

	void ScriptInstance::InvokeOnUpdate(float ts)
	{
		void* paramTs = &ts;
		m_ScriptClass->InvokeMethod(m_Instance, m_OnUpdateMethod, &paramTs, nullptr);
	}

	// Example
	void RunExample()
	{
#if 0
		// Retrieve and instanciate class (with contructor)
		s_Data->DemoClass = new ScriptClass("Base", "Demo");
		MonoObject* instance = s_Data->DemoClass->Instantiate();
		int value = 10;

		// Call method
		{
			MonoMethod* monoMethodMessage = s_Data->DemoClass->GetMethod("PrintMessage", 0);
			s_Data->DemoClass->InvokeMethod(monoMethodMessage, instance, nullptr, nullptr);
		}

		{
			void* param = &value;

			MonoMethod* monoMethodMessage = s_Data->DemoClass->GetMethod("PrintInt", 1);
			s_Data->DemoClass->InvokeMethod(monoMethodMessage, instance, &param, nullptr);
		}

		{
			int value2 = 1450;
			void* params[2] =
			{
				&value,
				&value2,
			};

			MonoMethod* monoMethodMessage = s_Data->DemoClass->GetMethod("PrintInts", 2);
			s_Data->DemoClass->InvokeMethod(monoMethodMessage, instance, params, nullptr);
		}

		{
			MonoString* monoMessageParam = mono_string_new(s_Data->AppDomain, "Hello world from C++ to C# to C++");
			void* monoMessageParamPtr = monoMessageParam;

			MonoMethod* monoMethodMessage = s_Data->DemoClass->GetMethod("PrintCustomMessage", 1);
			s_Data->DemoClass->InvokeMethod(monoMethodMessage, instance, &monoMessageParamPtr, nullptr);
}
#endif
	}

}
