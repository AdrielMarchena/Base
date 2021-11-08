#include "Scene.h"
#include "render/render2D.h"
#include "render/3d/Render3D.h"
#include "Entity.h"
#include "render/Camera.h"

#include "Components.h"
#include "utils/Instrumentor.h"
#include "ScriptableEntity.h"

#include "input/Keyboard.h"

#include "glm/gtc/matrix_transform.hpp"
#include "utils/gl_error_macro_db.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
namespace Base
{
	//TODO: temp
	static TransformComponent m_CameraTransform;
	static float s_value = 1.0f;

	namespace utils
	{
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
		int width = WindowProps().width;
		int height = WindowProps().height;
		
		FrameBufferRenderSpecification spec;
		spec.width = width;
		spec.height = height;
		spec.scale_factor = 1.0f;

		m_FrameBufferRender = MakeScope<FramebufferRender>(spec);
		SetFrameBuff(width, height, 1.0f);
		
	}

	Scene::~Scene()
	{
		m_FrameBufferRender.reset();
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
		entity.AddComponent<IDComponent>();
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

	void Scene::SetFrameBuff(unsigned int w, unsigned int h, float scale_factor)
	{

		s_value = scale_factor;
		FrameBufferRenderSpecification& spec = m_FrameBufferRender->GetSpec();
		spec.width = w;
		spec.height = h;
		spec.scale_factor = scale_factor;
		m_FrameBufferRender->InvalidateFrameBuffer();
		int width = WindowProps().width;
		int height = WindowProps().height;
		m_FramebufferCamera.SetViewportSize(width, height);
		float s = m_FramebufferCamera.GetOrthographicSize();
		m_FrameBufferRender->SetQuadScale({ s * B_GetRatio() * 1.0f,1.0f * s, 0.5f });
	}

	void Scene::SetPostEffect(const std::string& name)
	{
		m_FrameBufferRender->UsePostEffect(name);
	}

	void Scene::RuntimeInit()
	{
		m_PhysicWorld = new b2World({0.0f,-9.8f});
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
		}
	}

	void Scene::RuntimeStop()
	{
		if (m_PhysicWorld)
		{
			delete m_PhysicWorld;
			m_PhysicWorld = nullptr;
		}
	}

	const std::unordered_map<std::string, FramebufferPostEffect>& Scene::GetPostEffects() const
	{
		return m_FrameBufferRender->GetPostEffects();
	}

	void Scene::OnUpdate(const UpdateArgs& args)
	{
		BASE_PROFILE_FUNCTION();

		//TODO: The render will not bind any shader when the batch is full, fix this (Batch problem)
		// Fix: Binding the shader here before rendering things (maybe not the best solution)

		{//Native Scripts
			BASE_PROFILE_SCOPE("Script Updates");
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& script)
			{
				if (script.Instance)
					script.Instance->OnUpdate(args);
			});
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
			m_FrameBufferRender->BindFrameBuffer();
			
			int w = WindowProps().width;
			int h = WindowProps().height;
			 
			GLCall(glViewport(0, 0, w * s_value, h * s_value));
			
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
				D2D::BeginScene(*mainCamera2D, cameraTransform2D);

				{// Quads
					
					D2D::GetQuadShader()->Bind(); //Fix the batch problem, but maybe not the best solution
					D2D::BeginBatchQuads();
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
					D2D::EndBatchQuads();
					D2D::FlushQuads();
				}

				{// Circle
					
					D2D::GetCircleShader()->Bind();
					D2D::BeginBatchCircles();
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
					D2D::EndBatchCircles();
					D2D::FlushCircles();
				}

				//Finish the 2D rendering
				//D2D::EndBatch();
				//D2D::Flush();
			}
#ifdef BASE_USING_3D
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
#endif
			m_FrameBufferRender->UnbindFrameBuffer();
			GLCall(glViewport(0, 0, w, h));
			
		}//End Render Scope
		
		//Render3D::Clear();
		m_FrameBufferRender->DrawFrameBuffer(m_FramebufferCamera, m_CameraTransform.GetTransform());
	}
}