#include "Scene.h"
#include "Components.h"
#include "render/render2D.h"
namespace en
{
	struct r{};
	Scene::Scene()
	{
		entt::entity entity = m_Registry.create();
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
		auto group = m_Registry.group<PositionComponent>(entt::get<SpriteComponent>);
		for (auto entity : group)
		{
			//TODO: Make reference
			auto [position, spr] = group.get<PositionComponent, SpriteComponent>(entity);
			render::Render2D::DrawQuad(position.Position, position.Size, *spr.Texture);
		}
	}
}