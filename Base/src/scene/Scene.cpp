#include "Scene.h"
#include "render/render2D.h"
#include "Entity.h"
#include "render/Camera.h"

#include "Components.h"

namespace en
{
	static inline void SetTransform(float wid, float hei)
	{
		float ratio = wid / hei;
		render::Render2D::SetTransform(glm::ortho(0.0f, wid, 0.0f, hei, 1.0f, -10.0f));
	}

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::SceneBegin()
	{
		//Instanciate all Scripts
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& script)
		{
			if (!script.Instance)
			{
				script.Instance = script.InstantiateScript(); //Instanciate the script class inside
				script.Instance->m_Entity = Entity{ entity,this };
				script.Instance->OnCreate();
			}
		});
	}

	void Scene::SceneEnd()
	{
		//Destroy all Scripts
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& script)
		{
			if (!script.Instance)
			{
				script.DestroyScript(&script);
				script.Instance->OnDestroy();
			}
		});
		
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Unnamed Entity" : name;
		return entity;
	}

	void Scene::StartNativeScript(Entity& ent)
	{
		auto& script = ent.GetComponent<NativeScriptComponent>();
		if (!script.Instance)
		{
			script.Instance = script.InstantiateScript(); //Instanciate the script class inside
			script.Instance->m_Entity = ent;
			script.Instance->OnAwake();
		}
	}

	void Scene::OnUpdate(const UpdateArgs& args)
	{
		{//Native Scripts
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& script)
			{
				if (script.Instance)
					script.Instance->OnUpdate(args);
			});
		}

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

			{//Draw Sprites
				//It's a view because a group just breaks
				auto view = m_Registry.view<TransformComponent, SpriteComponent>();
				for (auto entity : view)
				{
					auto&& [position, spr] = view.get<TransformComponent, SpriteComponent>(entity);
					render::DrawQuad(position.Transform, spr.Color, spr.Rotation, spr.Axis);
				}
			}

			{//Draw Textures
				auto view = m_Registry.view<TransformComponent, TextureComponent>();
				for (auto entity : view)
				{
					auto&& [position, spr] = view.get<TransformComponent, TextureComponent>(entity);
					if (spr.Texture)
						render::DrawQuad(position.Transform, spr.Texture, Color::White, spr.Rotation, spr.Axis);
				}
			}

			{//Draw SebTextures
				auto view = m_Registry.view<TransformComponent, SubTextureComponent>();
				for (auto entity : view)
				{
					auto&& [position, spr] = view.get<TransformComponent, SubTextureComponent>(entity);
					if (spr.SubTexture)
						render::DrawQuad(position.Transform, spr.SubTexture, Color::White, spr.Rotation, spr.Axis);
				}
			}

			{//Draw Animated stuff
				auto view = m_Registry.view<TransformComponent, AnimateComponent>();
				for (auto entity : view)
				{
					//TODO: Test to see if works
					auto&& [position, anim] = view.get<TransformComponent, AnimateComponent>(entity);
					auto& sprite = anim.Animation.Run(args.dt);
					render::DrawQuad(position.Transform, sprite, Color::White, anim.Rotation, anim.Axis);
				}
			}

			{//Draw Circles
				auto view = m_Registry.view<CircleComponent>();
				for (auto entity : view)
				{
					auto& circle_def = view.get<CircleComponent>(entity);
					render::DrawCircle(circle_def.Position, circle_def.Radius, circle_def.Color, circle_def.Fill);
				}
			}

			//Finish the rendering
			render::EndBatch();
			render::Flush();
		}
	}
}