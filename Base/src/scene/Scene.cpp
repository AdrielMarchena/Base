#include "Scene.h"

namespace en
{
	struct r{};
	Scene::Scene()
	{
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<r>(entity);

	}

	Scene::~Scene()
	{
	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}
	void Scene::OnUpdate(const UpdateArgs& args)
	{
	}
}