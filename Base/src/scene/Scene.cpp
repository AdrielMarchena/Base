#include "Scene.h"
#include "render/render2D.h"
#include "Entity.h"
#include "render/Camera.h"
namespace en
{
	static inline void SetTransform(float wid, float hei)
	{
		float ratio = wid / hei;
		render::Render2D::SetTransform(glm::ortho(0.0f, wid, 0.0f, hei, 1.0f, -10.0f));
	}

	Scene::Scene()
	{
		//entt::entity entity = m_Registry.create();
		
		//m_S_Pointer_Scene = std::make_shared<Scene*>(this);
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Unnamed Entity" : name;
		return entity;
	}
	void Scene::OnUpdate(const UpdateArgs& args)
	{
		en::Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<CameraComponent>);
			for (auto entity : group)
			{
				auto&& [position, camera] = group.get< TransformComponent, CameraComponent>(entity);
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &position.Transform;
				}
			}
		}

		if (mainCamera)
		{
			using render = render::Render2D;
			//Clear screen
			render::ClearColorDepth();

			//Start render Scene
			SetTransform(800,600); //TODO: Remove this, maybe remove this Transform from everything
			render::BeginScene(*mainCamera,*cameraTransform);
			render::BeginBatch();

			//It's a view because SpriteComponent have std::shared_ptr inside (group dont like those)
			//I think a could remove them bu idk
			
			auto view = m_Registry.view<TransformComponent, SpriteComponent>();
			for (auto entity : view)
			{
				auto&& [position, spr] = view.get<TransformComponent, SpriteComponent>(entity);
				//glm::vec2 position = glm::vec2(pos.Transform[12], pos.Transform[13]);
				//glm::vec2 Size = { 120,102 }; //TODO: REMOVE THIS
				if (spr.Texture)
					render::Render2D::DrawQuad(position.Transform, spr.Texture, spr.Color, spr.Rotation, spr.Axis);
				else if (spr.SubTexture)
					render::Render2D::DrawQuad(position.Transform, spr.SubTexture, spr.Color, spr.Rotation, spr.Axis);
				else
					render::Render2D::DrawQuad(position.Transform, spr.Color, spr.Rotation, spr.Axis);
			}

			//Finish the rendering
			render::EndBatch();
			render::Flush();
		}
	}
}