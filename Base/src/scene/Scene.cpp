#include "Scene.h"
#include "render/render2D.h"
#include "Entity.h"
#include "render/Camera.h"

#include "Components.h"
#include "utils/Instrumentor.h"
namespace Base
{
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
		BASE_PROFILE_FUNCTION();

		{//Native Scripts
			BASE_PROFILE_SCOPE("Scene Native Script(OnUpdate)");
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& script)
			{
				if (script.Instance)
					script.Instance->OnUpdate(args);
			});
		}

		{//Render Scope
			BASE_PROFILE_SCOPE("Scene Render Scope");
			Base::Camera* mainCamera = nullptr;
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
				render::ClearColor(render::Cl_DepthColor);

				//Start render Scene
				//SetTransform(800,600); //TODO: Remove this, maybe remove this Transform from everything
				render::BeginScene(*mainCamera, *cameraTransform);
				render::BeginBatch();

				{//Draw Sprites
					//It's a view because a group just breaks
					auto view = m_Registry.view<TransformComponent, SpriteComponent>(entt::exclude<CircleComponent>);
					for (auto entity : view)
					{
						auto&& [position, spr] = view.get<TransformComponent, SpriteComponent>(entity);
						render::DrawQuad(position.Transform, spr.Color, spr.Rotation, spr.Axis);
					}
				}

				{//Draw SubTextures
					auto view = m_Registry.view<TransformComponent, TextureComponent>(entt::exclude<CircleComponent>);
					for (auto entity : view)
					{
						auto&& [position, spr] = view.get<TransformComponent, TextureComponent>(entity);
						if (spr.Texture)
							render::DrawQuad(position.Transform, spr.Texture, Color::White, spr.Rotation, spr.Axis);
					}
				}

				{//Draw SubTextures
					auto view = m_Registry.view<TransformComponent, SubTextureComponent>(entt::exclude<CircleComponent>);
					for (auto entity : view)
					{
						auto&& [position, spr] = view.get<TransformComponent, SubTextureComponent>(entity);
						if (spr.SubTexture)
							render::DrawQuad(position.Transform, spr.SubTexture, Color::White, spr.Rotation, spr.Axis);
					}
				}

				{//Draw Animated stuff
					auto view = m_Registry.view<TransformComponent, AnimateComponent>(entt::exclude<CircleComponent>);
					for (auto entity : view)
					{
						//TODO: Test to see if works
						auto&& [position, anim] = view.get<TransformComponent, AnimateComponent>(entity);
						auto& sprite = anim.Animation.Run(args.dt);
						render::DrawQuad(position.Transform, sprite, Color::White, anim.Rotation, anim.Axis);
					}
				}

				{//Draw Color Circles
					auto view = m_Registry.view<TransformComponent, CircleComponent, SpriteComponent>();
					for (auto entity : view)
					{
						auto&& [trans, circle_def, spr] = view.get<TransformComponent, CircleComponent, SpriteComponent>(entity);
						render::DrawCircle(trans.Transform, circle_def.Radius, circle_def.Fill, 1.0f, spr.Color);
					}
				}

				{//Draw Texture Circles
					auto view = m_Registry.view<TransformComponent, CircleComponent, TextureComponent>();
					for (auto entity : view)
					{
						auto&& [trans, circle_def, tex] = view.get<TransformComponent, CircleComponent, TextureComponent>(entity);
						render::DrawCircle(trans.Transform, circle_def.Radius, circle_def.Fill, 1.0f, tex.Texture, Color::White);
					}
				}

				//Finish the rendering
				render::EndBatch();
				render::Flush();
			}
			else
			{
				static bool once = []() //Just warn once
				{
					BASE_WARN("There is no Camera to use on rendering");
					return false;
				}();
			}
		}//End Render Scope
	}
}