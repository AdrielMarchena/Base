#include "Entity.h"

#include "Components.h"

namespace Base
{
	Entity::Entity(entt::entity handle, Scene* scene)
		:m_EntityHandle(handle), m_Scene(scene)
	{
	}

	Base::TransformComponent& Entity::GetTransform()
	{
		BASE_CORE_ASSERT(HasComponent<Base::TransformComponent>(), "Entity has no TransformComponent!");
		return m_Scene->m_Registry.get<Base::TransformComponent>(m_EntityHandle);
	}

	Base::TagComponent& Entity::GetTag()
	{
		BASE_CORE_ASSERT(HasComponent<Base::TagComponent>(), "Entity has no TagComponent!");
		return m_Scene->m_Registry.get<Base::TagComponent>(m_EntityHandle);
	}
}