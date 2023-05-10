#include "pch.h"
#include "ScriptEngine.h"

#include <glm/glm.hpp>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Base {

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
	};
	static ScriptEngineData* s_Data = nullptr;


	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();
		InitMono();
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_Data;
	}

	static void NativeLog(MonoString* text, int parameter)
	{
		char* t = mono_string_to_utf8(text);
		BASE_DEBUG_LOG("text: '{0}' parameter: '{1}'", t, parameter);
		mono_free(t);
	}

	static void NativeLogVector3(glm::vec3* parameter, glm::vec3* outResult)
	{
		BASE_WARN("Value: '[ {0}, {1}, {2} ]'", parameter->x, parameter->y, parameter->z);
		*outResult = glm::cross(*parameter, glm::vec3(parameter->x, parameter->y, -parameter->z));
	}

	MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath);
	void PrintAssemblyTypes(MonoAssembly* assembly);
	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("BaseJITRuntime");
		BASE_CORE_ASSERT(rootDomain, "Could not initialize Mono");

		s_Data->RootDomain = rootDomain;
		s_Data->AppDomain = mono_domain_create_appdomain((char*)"BaseDomain", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = LoadCSharpAssembly("Resources/Scripts/Base-ScriptCore.dll");
		PrintAssemblyTypes(s_Data->CoreAssembly);

		// create an object
		MonoImage* assemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		MonoClass* monoClass = mono_class_from_name(assemblyImage, "Base", "Main");

		mono_add_internal_call("Base.Main::NativeLog", NativeLog);
		mono_add_internal_call("Base.Main::NativeLogVector3", NativeLogVector3);

		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);

		MonoMethod* monoMethodMessage = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
		mono_runtime_invoke(monoMethodMessage, instance, nullptr, nullptr);

		int value = 10;
		void* param = &value;

		MonoMethod* monoMethodMessagePar = mono_class_get_method_from_name(monoClass, "PrintInt", 1);
		mono_runtime_invoke(monoMethodMessagePar, instance, &param, nullptr);

		int value2 = 1450;
		void* params[2] =
		{
			&value,
			&value2,
		};

		MonoMethod* monoMethodMessageParInts = mono_class_get_method_from_name(monoClass, "PrintInts", 2);
		mono_runtime_invoke(monoMethodMessageParInts, instance, params, nullptr);

		MonoString* monoMessageParam = mono_string_new(s_Data->AppDomain, "Hello world from C++ to C# to C++");
		void* monoMessageParamPtr = monoMessageParam;

		MonoMethod* monoMethodMessageCustom = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 1);
		mono_runtime_invoke(monoMethodMessageCustom, instance, &monoMessageParamPtr, nullptr);

	}

	void ScriptEngine::ShutdownMono()
	{
		// mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
		// mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;
	}

	char* ReadBytes(const std::string& filepath, uint32_t* outSize)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		BASE_CORE_ASSERT(stream, "Could not open file '{0}'", filepath);

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();

		BASE_CORE_ASSERT(size > 0, "The file '{0}' is empty", filepath);

		char* buffer = new char[size];
		stream.read((char*)buffer, size);
		stream.close();

		*outSize = size;
		return buffer;
	}

	MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
	{
		uint32_t fileSize = 0;
		char* fileData = ReadBytes(assemblyPath, &fileSize);

		// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

		BASE_CORE_ASSERT(status == MONO_IMAGE_OK, "Error on MonoImage, '{0}'", mono_image_strerror(status));

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		// Don't forget to free the file data
		delete[] fileData;

		return assembly;
	}

	void PrintAssemblyTypes(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			printf("%s.%s\n", nameSpace, name);
		}
	}
}
