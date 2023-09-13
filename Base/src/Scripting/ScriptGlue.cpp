#include "pch.h"
#include "ScriptGlue.h"

#include "Scripting/ScriptEngine.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

#include <mono/metadata/object.h>
#include <glm/glm.hpp>

namespace Base {
#define BASE_ADD_INTERNAL_CALL(icall) mono_add_internal_call("Base.InternalCalls::"#icall, (void*)icall)

#pragma region Demo

	static void NativeLog(MonoString* text, int parameter)
	{
		char* t = mono_string_to_utf8(text);
		BASE_DEBUG_LOG("text: '{0}' parameter: '{1}'", (void*)t, parameter);
		mono_free(t);
	}

	static void NativeLogVector3(glm::vec3* parameter, glm::vec3* outResult)
	{
		BASE_WARN("Value: '[ {0}, {1}, {2} ]'", parameter->x, parameter->y, parameter->z);
		*outResult = glm::cross(*parameter, glm::vec3(parameter->x, parameter->y, -parameter->z));
	}

	static void NativeLogVector3Dot(glm::vec3* parameter, glm::vec3* outResult)
	{
		BASE_WARN("Value: '[ {0}, {1}, {2} ]'", parameter->x, parameter->y, parameter->z);
		*outResult = glm::normalize(*parameter);
	}
#pragma endregion

	using namespace input;
	static void Entity_GetTranslation(UUID entityId, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();

		Entity entity = scene->GetEntityByUUID(entityId);
		if (entity)
		{
			*outTranslation = entity.GetTransform().Translation;
		}
	}

	static void Entity_SetTranslation(UUID entityId, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();

		Entity entity = scene->GetEntityByUUID(entityId);
		if (entity)
		{
			entity.GetTransform().Translation = *translation;
		}
	}

	static bool Input_IsKeyDown(int32_t key)
	{
		return Keyboard::isPress(key);
	}

	void ScriptGlue::RegisterFunctions()
	{
		BASE_ADD_INTERNAL_CALL(NativeLog);
		BASE_ADD_INTERNAL_CALL(NativeLogVector3);
		BASE_ADD_INTERNAL_CALL(NativeLogVector3Dot);

		BASE_ADD_INTERNAL_CALL(Entity_GetTranslation);
		BASE_ADD_INTERNAL_CALL(Entity_SetTranslation);

		BASE_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}