#pragma once
#include "Scene.h"
#include "entt/entt.hpp"
#include "utils/base_assert.h"

namespace Base
{
	struct TransformComponent;
	struct TagComponent;
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

		Base::TransformComponent& GetTransform();
		Base::TagComponent& GetTag();

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

		operator bool() const { return m_EntityHandle != entt::null; }

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}


