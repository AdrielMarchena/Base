#include "Scene.h"
#include "render/render2D.h"
#include "render/3d/Render3D.h"
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
			if (script.Instance)
			{
				script.Instance->OnDestroy();
				script.DestroyScript(&script);
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
			script.Instance->OnCreate();
		}
	}

	void Scene::DestroyNativeScript(Entity& ent)
	{
		auto& script = ent.GetComponent<NativeScriptComponent>();
		if (script.Instance)
		{
			script.Instance->OnDestroy();
			script.DestroyScript(&script); //Instanciate the script class inside
		}
	}

	void Scene::AwakeNativeScript(Entity& ent)
	{
		auto& script = ent.GetComponent<NativeScriptComponent>();
		if (script.Instance)
			script.Instance->OnAwake();
	}

	void Scene::OnUpdate(const UpdateArgs& args)
	{
		BASE_PROFILE_FUNCTION();

		//TODO: The render will not bind any shader when the batch is full, fix this (Batch problem)
		// Fix: Binding the shader here before rendering things (maybe not the best solution)

		{//Native Scripts
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& script)
			{
				if (script.Instance)
					script.Instance->OnUpdate(args);
			});
		}

		{//Render Scope
			BASE_PROFILE_SCOPE("Scene Render Scope");
			Base::Camera* mainCamera2D = nullptr;
			glm::mat4 cameraTransform2D;

			Base::Camera* mainCamera3D = nullptr;
			glm::mat4 cameraTransform3D;

			{
				auto group = m_Registry.group<TransformComponent>(entt::get<CameraComponent>);
				for (auto entity : group)
				{
					auto&& [position, camera] = group.get< TransformComponent, CameraComponent>(entity);
					if (camera.Primary && camera.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
					{
						mainCamera2D = &camera.Camera;
						cameraTransform2D = position.GetTransform();
					}
					if (camera.Primary && camera.Camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
					{
						mainCamera3D = &camera.Camera;
						cameraTransform3D = position.GetTransform();
					}
				}
			}

			using D2D = render::Render2D;
			using D3D = Render3D;

			D3D::Clear();
			if (mainCamera2D)
			{
				//Clear scree
				//TODO: for now the 3D clear do the job for 2D as well

				//Start render Scene
				//SetTransform(800,600); //TODO: Remove this, maybe remove this Transform from everything
				D2D::BeginScene(*mainCamera2D, cameraTransform2D);
				D2D::BeginBatch();

				{// Quads
					
					D2D::GetQuadShader()->Bind(); //Fix the batch problem, but maybe not the best solution
					
					{//Draw Sprites
						//It's a view because a group just breaks
						auto view = m_Registry.view<TransformComponent, SpriteComponent>(entt::exclude<CircleComponent>);
						for (auto entity : view)
						{
							auto&& [position, spr] = view.get<TransformComponent, SpriteComponent>(entity);
							D2D::DrawQuad(position.GetTransform(), spr.Color, spr.Rotation, spr.Axis);
						}
					}

					//TODO: remove
					m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& script)
						{
							if (script.Instance)
								script.Instance->ExtraRender();
						});


					{//Draw Textures
						auto view = m_Registry.view<TransformComponent, TextureComponent>(entt::exclude<CircleComponent>);
						for (auto entity : view)
						{
							auto&& [position, spr] = view.get<TransformComponent, TextureComponent>(entity);
							if (spr.Texture)
								D2D::DrawQuad(position.GetTransform(), spr.Texture, Color::White, spr.Rotation, spr.Axis);
						}
					}

					{//Draw SubTextures
						auto view = m_Registry.view<TransformComponent, SubTextureComponent>(entt::exclude<CircleComponent>);
						for (auto entity : view)
						{
							auto&& [position, spr] = view.get<TransformComponent, SubTextureComponent>(entity);
							if (spr.SubTexture)
								D2D::DrawQuad(position.GetTransform(), spr.SubTexture, Color::White, spr.Rotation, spr.Axis);
						}
					}

					{//Draw Animated stuff
						auto view = m_Registry.view<TransformComponent, AnimateComponent>(entt::exclude<CircleComponent>);
						for (auto entity : view)
						{
							//TODO: Test to see if works
							auto&& [position, anim] = view.get<TransformComponent, AnimateComponent>(entity);
							auto& sprite = anim.Animation.Run(args.dt);
							D2D::DrawQuad(position.GetTransform(), sprite, Color::White, anim.Rotation, anim.Axis);
						}
					}
				}

				{// Circle
					
					D2D::GetCircleShader()->Bind();
					
					{//Draw Color Circles
						auto view = m_Registry.view<TransformComponent, CircleComponent, SpriteComponent>();
						for (auto entity : view)
						{
							auto&& [trans, circle_def, spr] = view.get<TransformComponent, CircleComponent, SpriteComponent>(entity);
							trans.Scale = glm::vec3(circle_def.Radius);
							D2D::DrawCircle(trans.GetTransform(), circle_def.Radius, circle_def.Fade, circle_def.Thickness, spr.Color);
						}
					}

					{//Draw Texture Circles
						auto view = m_Registry.view<TransformComponent, CircleComponent, TextureComponent>();
						for (auto entity : view)
						{
							auto&& [trans, circle_def, tex] = view.get<TransformComponent, CircleComponent, TextureComponent>(entity);
							trans.Scale = glm::vec3(circle_def.Radius);
							D2D::DrawCircle(trans.GetTransform(), circle_def.Radius, circle_def.Fade, circle_def.Thickness, tex.Texture, Color::White);
						}
					
					}
				}

				//Finish the rendering
				D2D::EndBatch();
				D2D::Flush();
			}
			if (mainCamera3D)
			{
				D3D::StartScene(*mainCamera3D, cameraTransform3D);
				D3D::StartBatch();

				{//Draw Models
					auto view = m_Registry.view<TransformComponent, ModelComponent>();
					for (auto entity : view)
					{
						auto&& [trans, model] = view.get< TransformComponent, ModelComponent>(entity);
						D3D::SubmitMesh(model.Model3D->GetMeshes(), trans.GetTransform());
					}
				}
				D3D::EndBatch();
				D3D::Flush();
				D3D::EndScene();
			}
		}//End Render Scope
	}
}