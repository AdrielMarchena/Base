#include "pch.h"
#include "ScriptGlue.h"

#include "Scripting/ScriptEngine.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <glm/glm.hpp>
#include <box2d/b2_body.h>

namespace Base {

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;
	static std::unordered_map<MonoType*, std::function<void(Entity)>> s_EntityAddComponentFuncs;
	static std::unordered_map<MonoType*, std::function<void(Entity)>> s_EntityRemoveComponentFuncs;
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

	namespace Utils {
		Scene* GetScene()
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			BASE_CORE_ASSERT(scene != nullptr, "There is no current scene set, failed on Utils::GetScene");
			return scene;
		}

		Entity GetEntity(UUID entityId)
		{
			Entity entity = GetScene()->GetEntityByUUID(entityId);
			BASE_CORE_ASSERT(entity, "There is no entity with id '{0}', failed on Utils::GetEntity", entityId);
			return entity;
		}

		MonoType* GetManagedType(MonoReflectionType* componentType)
		{
			MonoType* managedType = mono_reflection_type_get_type(componentType);
			BASE_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end(), "Type is not registered, failed on Entity_HasComponent");
			return managedType;
		}
	}

	using namespace input;

#pragma region TransformComponent
	static void TransformComponent_GetTranslation(UUID entityId, glm::vec3* outTranslation)
	{
		Entity entity = Utils::GetEntity(entityId);
		*outTranslation = entity.GetTransform().Translation;
	}

	static void TransformComponent_SetTranslation(UUID entityId, glm::vec3* translation)
	{
		Entity entity = Utils::GetEntity(entityId);
		if (entity)
		{
			entity.GetTransform().Translation = *translation;
		}
	}
#pragma endregion

#pragma region Entity
	static bool Entity_HasComponent(UUID entityId, MonoReflectionType* componentType)
	{
		MonoType* managedType = Utils::GetManagedType(componentType);

		Entity entity = Utils::GetEntity(entityId);
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static void Entity_RemoveComponent(UUID entityId, MonoReflectionType* componentType)
	{
		MonoType* managedType = Utils::GetManagedType(componentType);

		Entity entity = Utils::GetEntity(entityId);
		return s_EntityRemoveComponentFuncs.at(managedType)(entity);
	}

	static void Entity_AddComponent(UUID entityId, MonoReflectionType* componentType)
	{
		MonoType* managedType = Utils::GetManagedType(componentType);

		Entity entity = Utils::GetEntity(entityId);
		return s_EntityAddComponentFuncs.at(managedType)(entity);
	}
#pragma endregion

#pragma region RigidBody2DComponent
	static void RigidBody2DComponent_SetImpulse(UUID entityId, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		Entity entity = Utils::GetEntity(entityId);
		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	static void RigidBody2DComponent_SetImpulseToCenter(UUID entityId, glm::vec2* impulse, bool wake)
	{
		Entity entity = Utils::GetEntity(entityId);
		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}
#pragma endregion

#pragma region Scene
	static void Scene_CreateEntityWithUUID(UUID entityId, MonoString* entityName)
	{
		std::string name = entityName != nullptr ? mono_string_to_utf8(entityName) : "";
		Utils::GetScene()->CreateEntityWhithUUID(entityId, name);
	}

	static UUID Scene_CreateEntity(MonoString* entityName)
	{
		std::string name = entityName != nullptr ? mono_string_to_utf8(entityName) : "";
		Entity entity = Utils::GetScene()->CreateEntity(name);
		return entity.GetID();
	}
#pragma endregion
	
#pragma region Input
	static bool Input_IsKeyDown(int32_t key)
	{
		return Keyboard::isPress(key);
	}
#pragma endregion

#pragma region RegisterFunctions
	void ScriptGlue::RegisterFunctions()
	{
		BASE_ADD_INTERNAL_CALL(NativeLog);
		BASE_ADD_INTERNAL_CALL(NativeLogVector3);
		BASE_ADD_INTERNAL_CALL(NativeLogVector3Dot);

		BASE_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		BASE_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

		BASE_ADD_INTERNAL_CALL(Entity_HasComponent);
		BASE_ADD_INTERNAL_CALL(Entity_AddComponent);
		BASE_ADD_INTERNAL_CALL(Entity_RemoveComponent);

		BASE_ADD_INTERNAL_CALL(RigidBody2DComponent_SetImpulse);
		BASE_ADD_INTERNAL_CALL(RigidBody2DComponent_SetImpulseToCenter);

		BASE_ADD_INTERNAL_CALL(Scene_CreateEntityWithUUID);
		BASE_ADD_INTERNAL_CALL(Scene_CreateEntity);

		BASE_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}

	template<typename... T>
	void RegisterComponent(MonoImage* image)
	{
		([image]()
		{
			std::string name = fmt::format("Base.{}", T::StructName);
			MonoType* managedType = mono_reflection_type_from_name(name.data(), image);
			// BASE_CORE_ASSERT(managedType, "Managed type '{0}' does not exists on script", name);
			BASE_ERROR("Managed type '{0}' does not exists on script", name);
			s_EntityHasComponentFuncs[managedType] = [](Entity entity) -> bool { return entity.HasComponent<T>(); };
			s_EntityAddComponentFuncs[managedType] = [](Entity entity) -> void { entity.AddComponent<T>(); };
			s_EntityRemoveComponentFuncs[managedType] = [](Entity entity) -> void { return entity.RemoveComponent<T>(); };
		}(), ...);
	}

	template<typename... T>
	void RegisterComponent(ComponentGroup<T...> g, MonoImage* image)
	{
		RegisterComponent <T...>(image);
	}

	void ScriptGlue::RegisterComponents()
	{
		MonoImage* image = ScriptEngine::GetCoreAssemblyImage();

		RegisterComponent(AllComponent{}, image);
	}
#pragma endregion
}
