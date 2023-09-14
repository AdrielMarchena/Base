#include "pch.h"
#include "Scene.h"
#include "render/render2D.h"
#include "Entity.h"
#include "render/Camera.h"

#include "Components.h"
#include "utils/Instrumentor.h"
#include "ScriptableEntity.h"
#include "Scripting/ScriptEngine.h"

#include "input/Keyboard.h"

#include "glm/gtc/matrix_transform.hpp"
#include "utils/gl_error_macro_db.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"

#include "meta/factory.hpp"
#include "meta/meta.hpp"

#include <concepts>
namespace Base {
	namespace utils {
		static inline b2BodyType Get2DBodyType(RigidBody2DComponent::BodyType type)
		{
			switch (type)
			{
			case RigidBody2DComponent::BodyType::Static: return b2BodyType::b2_staticBody;
			case RigidBody2DComponent::BodyType::Dynamic: return b2BodyType::b2_dynamicBody;
			case RigidBody2DComponent::BodyType::Kinematic: return b2BodyType::b2_kinematicBody;
			}
			BASE_CORE_ASSERT(false, "Unknow body type");
			return b2BodyType::b2_staticBody;
		}
	}

	Scene::Scene()
	{
		m_ViewPortWidth = WindowProps().width;
		m_ViewPortHeight = WindowProps().height;

		OnViewPortResize(m_ViewPortWidth, m_ViewPortHeight);
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
		return CreateEntityWhithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWhithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Unnamed Entity" : name;
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();

		m_EntityMap[uuid] = entity;

		return entity;
	}

	void Scene::DestroyEntity(Entity ent)
	{
		m_Registry.destroy(ent);
		m_EntityMap.erase(ent.GetID());
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

	Entity Scene::GetPrimaryCamera()
	{
		auto view = m_Registry.view<CameraComponent>();
		Entity cam = {};
		for (auto en : view)
		{
			Entity handler = { en,this };
			if (handler.GetComponent<CameraComponent>().Primary)
			{
				cam = handler;
				break;
			}
		}
		return cam;
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
		{
			return { m_EntityMap.at(uuid), this };
		}
		return {};
	}

	void Scene::RuntimeInit()
	{
		m_PhysicWorld = new b2World({ 0.0f,-9.8f });
		auto view = m_Registry.view<RigidBody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

			b2BodyDef bodyDef;

			bodyDef.type = utils::Get2DBodyType(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;


			b2Body* body = m_PhysicWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxColider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxColider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}

			if (entity.HasComponent<CircleColider2DComponent>())
			{
				auto& cc2d = entity.GetComponent<CircleColider2DComponent>();

				b2CircleShape circleShape;
				circleShape.m_p.SetZero();
				circleShape.m_radius = cc2d.Radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = cc2d.Density;
				fixtureDef.friction = cc2d.Friction;
				fixtureDef.restitution = cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}

		//Script
		{
			ScriptEngine::OnRuntimeStart(this);

			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { e,this };
				ScriptEngine::OnCreateEntity(entity);
			}
		}
	}

	void Scene::RuntimeStop()
	{
		ScriptEngine::OnRuntimeStop();
		if (m_PhysicWorld)
		{
			delete m_PhysicWorld;
			m_PhysicWorld = nullptr;
		}
	}

	void Scene::OnUpdateEditor(const UpdateArgs& args, EditorCamera& camera)
	{
		BASE_PROFILE_FUNCTION();
		using D2D = Render2D;

		D2D::BeginScene(camera);
		D2D::BeginBatch();
		//D2D::SetLineWidth(2.0f);

		{// Text
			BASE_PROFILE_SCOPE("Text Render (Editor)");
			{// Text2D
				auto view = m_Registry.view<TransformComponent, Text2DComponent>();
				for (auto entity : view)
				{
					auto&& [transform, text] = view.get<TransformComponent, Text2DComponent>(entity);

					TransformComponent com = transform;
					for (auto c : text.Text)
					{
						D2D::DrawGlyph(com.GetTransform(), c, text.font, Color::White, (int)entity);

						double adv = text.font->GetGlyphsList()[c].Advance;
						if (adv != 0.0)
							com.Translation.x += (adv / adv) * com.Scale.x;
					}
				}
			}
		}

		{// Quads
			BASE_PROFILE_SCOPE("Render (Editor)");
			{//Draw Sprites
				//It's a view because a group just breaks
				auto view = m_Registry.view<TransformComponent, SpriteComponent>(entt::exclude<CircleComponent>);
				for (auto entity : view)
				{
					auto&& [position, spr] = view.get<TransformComponent, SpriteComponent>(entity);
					if (spr.Texture)
						D2D::DrawQuad(position.GetTransform(), spr.Texture, (int)entity, spr.Color);
					else
						D2D::DrawQuad(position.GetTransform(), spr.Color, (int)entity);
				}
			}

			{//Draw SubTextures
				auto view = m_Registry.view<TransformComponent, SubTextureComponent>(entt::exclude<CircleComponent>);
				for (auto entity : view)
				{
					auto&& [position, spr] = view.get<TransformComponent, SubTextureComponent>(entity);
					if (spr.SubTexture)
						D2D::DrawQuad(position.GetTransform(), spr.SubTexture, (int)entity);
				}
			}

			{//Draw Animated stuff
				auto view = m_Registry.view<TransformComponent, AnimateComponent>(entt::exclude<CircleComponent>);
				for (auto entity : view)
				{
					//TODO: Test to see if works
					auto&& [position, anim] = view.get<TransformComponent, AnimateComponent>(entity);
					auto& sprite = anim.Animation.Run(args.dt);
					D2D::DrawQuad(position.GetTransform(), sprite, (int)entity);
				}
			}
		}

		{// Circle
			{//Draw Color Circles
				auto view = m_Registry.view<TransformComponent, CircleComponent, SpriteComponent>();
				for (auto entity : view)
				{
					auto&& [trans, circle_def, spr] = view.get<TransformComponent, CircleComponent, SpriteComponent>(entity);
					//trans.Scale = glm::vec3(circle_def.Radius);
					if(spr.Texture)
						D2D::DrawCircle(trans.GetTransform(), circle_def.Radius, circle_def.Fade, circle_def.Thickness, spr.Texture, (int)entity);
					else
						D2D::DrawCircle(trans.GetTransform(), circle_def.Radius, circle_def.Fade, circle_def.Thickness, spr.Color, (int)entity);
				}
			}
		}

		//Finish the 2D rendering
		D2D::EndBatch();
		D2D::Flush();

	}

	void Scene::OnUpdateRuntime(const UpdateArgs& args)
	{
		BASE_PROFILE_FUNCTION();

		{//Native Scripts
			BASE_PROFILE_SCOPE("Script Updates");
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& script)
			{
				if (script.Instance)
					script.Instance->OnUpdate(args);
			});
		}

		//Script
		{
			ScriptEngine::OnRuntimeStart(this);

			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { e,this };
				ScriptEngine::OnUpdateEntity(entity, args.dt);
			}
		}

		if (m_PhysicWorld)
		{//Physics
			BASE_PROFILE_SCOPE("2D Physic's Updates");
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;

			m_PhysicWorld->Step(args.dt, velocityIterations, positionIterations);

			auto view = m_Registry.view<RigidBody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

				auto& transform = entity.GetTransform();
				b2Body* body = (b2Body*)rb2d.RuntimeBody;
				const auto& position = body->GetPosition();
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z = body->GetAngle();
			}
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

			using D2D = Render2D;
			D2D::ClearColor();
			if (mainCamera2D)
			{
				//Start render Scene
				D2D::BeginScene(*mainCamera2D, cameraTransform2D);
				D2D::BeginBatch();

				{// Text
					{// Text2D
						auto view = m_Registry.view<TransformComponent, Text2DComponent>();
						for (auto entity : view)
						{
							auto&& [transform, text] = view.get<TransformComponent, Text2DComponent>(entity);

							D2D::DrawFont(transform.GetTransform(), text.Text, text.font, Color::White, (int)entity);
						}
					}
				}

				{// Quads

					{//Draw Sprites
						//It's a view because a group just breaks
						auto view = m_Registry.view<TransformComponent, SpriteComponent>(entt::exclude<CircleComponent>);
						for (auto entity : view)
						{
							auto&& [position, spr] = view.get<TransformComponent, SpriteComponent>(entity);

							if(spr.Texture)
								D2D::DrawQuad(position.GetTransform(), spr.Texture ,(int)entity, spr.Color);
							else
								D2D::DrawQuad(position.GetTransform(), spr.Color, (int)entity);

							// glm::vec4 out_color = { 1.0f,0.0f ,0.0f ,1.0f };
							// D2D::DrawOutLineQuad(position.GetTransform(), out_color, (int)entity);
						}
					}

					{//Draw SubTextures
						auto view = m_Registry.view<TransformComponent, SubTextureComponent>(entt::exclude<CircleComponent>);
						for (auto entity : view)
						{
							auto&& [position, spr] = view.get<TransformComponent, SubTextureComponent>(entity);
							if (spr.SubTexture)
								D2D::DrawQuad(position.GetTransform(), spr.SubTexture, (int)entity);
						}
					}

					{//Draw Animated stuff
						auto view = m_Registry.view<TransformComponent, AnimateComponent>(entt::exclude<CircleComponent>);
						for (auto entity : view)
						{
							//TODO: Test to see if works
							auto&& [position, anim] = view.get<TransformComponent, AnimateComponent>(entity);
							auto& sprite = anim.Animation.Run(args.dt);
							D2D::DrawQuad(position.GetTransform(), sprite, (int)entity);
						}
					}
				}

				{// Cirvcle
					auto view = m_Registry.view<TransformComponent, CircleComponent, SpriteComponent>();
					for (auto entity : view)
					{
						auto&& [trans, circle_def, spr] = view.get<TransformComponent, CircleComponent, SpriteComponent>(entity);
						//trans.Scale = glm::vec3(circle_def.Radius);
						if (spr.Texture)
							D2D::DrawCircle(trans.GetTransform(), circle_def.Radius, circle_def.Fade, circle_def.Thickness, spr.Texture, (int)entity);
						else
							D2D::DrawCircle(trans.GetTransform(), circle_def.Radius, circle_def.Fade, circle_def.Thickness, spr.Color, (int)entity);
					}
				}

				//Finish the 2D rendering
				D2D::EndBatch();
				D2D::Flush();
			}
		}//End Render Scope
	}

	void Scene::OnViewPortResize(uint32_t w, uint32_t h)
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			if (!camera.FixedAspectRatio)
			{
				camera.Camera.SetViewportSize(w, h);
			}
		}

		m_ViewPortWidth = w;
		m_ViewPortHeight = h;
	}

	template<typename U, Derived<Component<U>> T>
	void Scene::OnComponentAdded(Entity ent, T& component)
	{
		BASE_ERROR("This generic templated function should not be called");
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity ent, IDComponent& component)
	{
		BASE_TRACE("'{0}' Added to Entity {1}", IDComponent::ComponentName, ent.GetTag());
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity ent, TagComponent& component)
	{
		BASE_TRACE("'{0}' Added to Entity {1}", TagComponent::ComponentName, ent.GetTag());
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity ent, TransformComponent& component)
	{
		BASE_TRACE("'{0}' Added to Entity {1}", TransformComponent::ComponentName, ent.GetTag());
	}

	template<>
	void Scene::OnComponentAdded<AnimateComponent>(Entity ent, AnimateComponent& component)
	{
		BASE_TRACE("'{0}' Added to Entity {1}", AnimateComponent::ComponentName, ent.GetTag());
	}

	template<>
	void Scene::OnComponentAdded<SubTextureComponent>(Entity ent, SubTextureComponent& component)
	{
		BASE_TRACE("'{0}' Added to Entity {1}", SubTextureComponent::ComponentName, ent.GetTag());
	}

	template<>
	void Scene::OnComponentAdded<SpriteComponent>(Entity ent, SpriteComponent& component)
	{
		BASE_TRACE("'{0}' Added to Entity {1}", SpriteComponent::ComponentName, ent.GetTag());
	}

	template<>
	void Scene::OnComponentAdded<CircleComponent>(Entity ent, CircleComponent& component)
	{
		BASE_TRACE("'{0}' Added to Entity {1}", CircleComponent::ComponentName, ent.GetTag());
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity ent, CameraComponent& component)
	{
		BASE_TRACE("'{0}' Added to Entity {1}", CameraComponent::ComponentName, ent.GetTag());
		component.Camera.SetViewportSize(m_ViewPortWidth, m_ViewPortHeight);
	}

	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Entity ent, ScriptComponent& component)
	{
		BASE_TRACE("'{0}' Added to Entity {1}", ScriptComponent::ComponentName, ent.GetTag());
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity ent, NativeScriptComponent& component)
	{
		BASE_TRACE("'{0}' Added to Entity {1}", NativeScriptComponent::ComponentName, ent.GetTag());
	}

	template<>
	void Scene::OnComponentAdded<RigidBody2DComponent>(Entity ent, RigidBody2DComponent& component)
	{
		BASE_TRACE("'{0}' Added to Entity {1}", RigidBody2DComponent::ComponentName, ent.GetTag());
	}

	template<>
	void Scene::OnComponentAdded<BoxColider2DComponent>(Entity ent, BoxColider2DComponent& component)
	{
		BASE_TRACE("'{0}' Added to Entity {1}", BoxColider2DComponent::ComponentName, ent.GetTag());
	}

	template<>
	void Scene::OnComponentAdded<CircleColider2DComponent>(Entity ent, CircleColider2DComponent& component)
	{
		BASE_TRACE("'{0}' Added to Entity {1}", CircleColider2DComponent::ComponentName, ent.GetTag());
	}

	template<>
	void Scene::OnComponentAdded<Text2DComponent>(Entity ent, Text2DComponent& component)
	{
		BASE_TRACE("'{0}' Added to Entity {1}", Text2DComponent::ComponentName, ent.GetTag());
	}

	template<>
	void Scene::OnComponentAdded<Perlin2dComponent>(Entity ent, Perlin2dComponent& component)
	{
		BASE_TRACE("'{0}' Added to Entity {1}", Perlin2dComponent::ComponentName, ent.GetTag());
	}
}