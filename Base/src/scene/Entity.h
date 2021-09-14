#pragma once
#include "Scene.h"
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
			BASE_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<_Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			BASE_CORE_ASSERT(HasComponent<T>(),"Entity does not have component!");
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
			BASE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}


