#pragma once
#include "Scene.h"
#include "Components.h"
#include "utils/base_assert.h"
#include "entt/entt.hpp"
namespace Base
{
	
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... _Args>
		T& AddComponent(_Args&&... args)
		{
			BASE_CORE_ASSERT(!HasComponent<T>(), "Entity already has component '{0}'! Try use .GetComponent<{0}>()",BASE_GET_PARSE_TYPE_NAME(T));
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<_Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			BASE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component '{0}'!", BASE_GET_PARSE_TYPE_NAME(T));
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			bool r = m_Scene->m_Registry.any_of<T>(m_EntityHandle);
			return r;
		}

		template<typename T>
		void RemoveComponent()
		{
			BASE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component '{0}'!", BASE_GET_PARSE_TYPE_NAME(T));
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		TransformComponent& Entity::GetTransform()
		{
			BASE_CORE_ASSERT(HasComponent<Base::TransformComponent>(), "Entity has no TransformComponent!");
			return m_Scene->m_Registry.get<Base::TransformComponent>(m_EntityHandle);
		}

		const std::string& Entity::GetTag()
		{
			BASE_CORE_ASSERT(HasComponent<Base::TagComponent>(), "Entity has no TagComponent!");
			return m_Scene->m_Registry.get<Base::TagComponent>(m_EntityHandle).Tag;
		}

		UUID_T Entity::GetID()
		{
			BASE_CORE_ASSERT(HasComponent<Base::IDComponent>(), "Entity has no IDComponent!");
			return m_Scene->m_Registry.get<Base::IDComponent>(m_EntityHandle).Id;
		}

		operator bool() const { return m_EntityHandle != entt::null; }

		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}


