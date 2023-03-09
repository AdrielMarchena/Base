#include "pch.h"
#include "Entity.h"

#include "Components.h"

namespace Base
{
	Entity::Entity(entt::entity handle, Scene* scene)
		:m_EntityHandle(handle), m_Scene(scene)
	{
	}

	void Entity::SetChild(Entity* child)
	{
		BASE_CORE_ASSERT(child->m_Parent, "Child entity ({0}) already has a parent ({1})", this->GetTag(), child->m_Parent->GetTag());
		m_Child = child;
		child->m_Parent = this;
	}
}