#pragma once

#include "Base.h"
#include "Scene.h"
#include "Components.h"
#include "utils/base_assert.h"
#include "entt/entt.hpp"

#include <concepts>
namespace Base {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... _Args> requires Derived<T, Component<T>>
		[[reason_not_used("T must inherit from 'Component<T>'")]]
		T& AddComponent(_Args&&... args)
		{
			BASE_CORE_ASSERT(!HasComponent<T>(), "Entity already has component '{0}'! Try use .GetComponent<{0}>()", BASE_GET_PARSE_TYPE_NAME(T));
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<_Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T> requires Derived<T, Component<T>>
		T& GetComponent()
		{
			BASE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component '{0}'!", BASE_GET_PARSE_TYPE_NAME(T));
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T> requires Derived<T, Component<T>>
		bool HasComponent()
		{
			bool r = m_Scene->m_Registry.any_of<T>(m_EntityHandle);
			return r;
		}

		template<typename T> requires Derived<T, Component<T>>
		void RemoveComponent()
		{
			BASE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component '{0}'!", BASE_GET_PARSE_TYPE_NAME(T));
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		TransformComponent& GetTransform()
		{
			BASE_CORE_ASSERT(HasComponent<TransformComponent>(), "Entity has no TransformComponent!");
			return m_Scene->m_Registry.get<Base::TransformComponent>(m_EntityHandle);
		}

		const std::string& GetTag()
		{
			BASE_CORE_ASSERT(HasComponent<Base::TagComponent>(), "Entity has no TagComponent!");
			return m_Scene->m_Registry.get<Base::TagComponent>(m_EntityHandle).Tag;
		}

		UUID_T GetID()
		{
			BASE_CORE_ASSERT(HasComponent<Base::IDComponent>(), "Entity has no IDComponent!");
			return m_Scene->m_Registry.get<Base::IDComponent>(m_EntityHandle).Id;
		}

		void SetChild(Entity* child);

		// void SetParent(Entity* parent)
		// {
		// }

		bool HasScene() const
		{
			return m_Scene != nullptr;
		}

		bool HasChild() const
		{
			return m_Child != nullptr;
		}

		bool HasParent() const
		{
			return m_Parent != nullptr;
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		explicit operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
		Entity* m_Child;
		Entity* m_Parent;
	};
}
