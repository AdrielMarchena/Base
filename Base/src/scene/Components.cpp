#include "pch.h"
#include "Scene.h"
#include "Components.h"

#include "entt/entt.hpp"

#include "meta/meta.hpp"
#include "meta/factory.hpp"

static bool s_Init = false;
namespace Base
{
	void Init::InitComponentsReflection()
	{
		return; //TODO: Fix this hole thing

		//TODO: understand this reflection thing of meta lib
		if (!s_Init)
		{
			UUID::MetaInit();

			std::hash<std::string_view> hash{};
			{
				//UUID
				auto factory = entt::meta<IDComponent>().type(hash("IDComponent"));
				factory.
					data<&IDComponent::Id>(hash("Id"));
				factory.ctor<UUID>();
				factory.ctor<>();
				factory.ctor<const IDComponent&>();
				//TODO: remove testing
				auto by_id = entt::resolve(hash("IDComponent"));

				auto prop = by_id.prop(hash("Id"));

				BASE_DEBUG_LOG(prop.value().cast<std::string>());

				//BASE_DEBUG_LOG(by_id.info().name());
				/*for (const auto& data : by_id.data())
				{
					data("Id");
					BASE_DEBUG_LOG(data.id());
					BASE_DEBUG_LOG(entt::resolve(hash("UUID")).id());
					BASE_DEBUG_LOG(data.type().id());
				}
				debug_break();*/
			}
			
			{
				//TagComp
				auto factory = entt::meta<TagComponent>().type(hash("TagComponent"));
				factory.
					data<&TagComponent::Tag>(hash("Tag"));
			}

			{
				//TransformComp
				auto factory = entt::meta<TransformComponent>().type(hash("TransformComponent"));
				factory.
					data<&TransformComponent::Translation>(hash("Translation")).
					data<&TransformComponent::Rotation>(hash("Rotation")).
					data<&TransformComponent::Scale>(hash("Scale"));
			}

			{
				//TextureComponent
				auto factory = entt::meta<TextureComponent>().type(hash("TextureComponent"));
				factory.
					data<&TextureComponent::Texture>(hash("Texture"));
			}

			{
				//SubTextureComponent
				auto factory = entt::meta<SubTextureComponent>().type(hash("SubTextureComponent"));
				factory.
					data<&SubTextureComponent::SubTexture>(hash("SubTexture"));
			}

			{
				//SpriteComponent
				auto factory = entt::meta<SpriteComponent>().type(hash("SpriteComponent"));
				factory.
					data<&SpriteComponent::Color>(hash("Color"));
			}

			{
				//CircleComponent
				auto factory = entt::meta<CircleComponent>().type(hash("CircleComponent"));
				factory.
					data<&CircleComponent::Radius>(hash("Radius")).
					data<&CircleComponent::Thickness>(hash("Thickness")).
					data<&CircleComponent::Fade>(hash("Fade"));
			}

			{
				//CameraComponent
				auto factory = entt::meta<CameraComponent>().type(hash("CameraComponent"));
				factory.
					data<&CameraComponent::Camera>(hash("Camera")).
					data<&CameraComponent::Primary>(hash("Primary")).
					data<&CameraComponent::FixedAspectRatio>(hash("FixedAspectRatio"));
			}

			/* { //TODO: Why?
				//NativeScriptComponent
				auto factory = entt::meta<NativeScriptComponent>().type(hash("NativeScriptComponent"));
				factory.
					data<&NativeScriptComponent::Instance>(hash("Instance"));
			}*/

			{
				//RigidBody2DComponent
				auto factory = entt::meta<RigidBody2DComponent>().type(hash("RigidBody2DComponent"));
				factory.
					data<&RigidBody2DComponent::Type>(hash("Type")).
					data<&RigidBody2DComponent::FixedRotation>(hash("FixedRotation")).
					data<&RigidBody2DComponent::RuntimeBody>(hash("RuntimeBody"));
			}

			{
				//BoxColider2DComponent
				auto factory = entt::meta<BoxColider2DComponent>().type(hash("BoxColider2DComponent"));
				factory.
					data<&BoxColider2DComponent::Offset>(hash("Offset")).
					data<&BoxColider2DComponent::Size>(hash("Size")).
					data<&BoxColider2DComponent::Density>(hash("Density")).
					data<&BoxColider2DComponent::Friction>(hash("Friction")).
					data<&BoxColider2DComponent::Restitution>(hash("Restitution")).
					data<&BoxColider2DComponent::RestitutionThreshold>(hash("RestitutionThreshold")).
					data<&BoxColider2DComponent::RuntimeFixture>(hash("RuntimeFixture"));
			}

			{
				//CircleColider2DComponent
				auto factory = entt::meta<CircleColider2DComponent>().type(hash("CircleColider2DComponent"));
				factory.
					data<&CircleColider2DComponent::Offset>(hash("Offset")).
					data<&CircleColider2DComponent::Size>(hash("Size")).
					data<&CircleColider2DComponent::Density>(hash("Density")).
					data<&CircleColider2DComponent::Friction>(hash("Friction")).
					data<&CircleColider2DComponent::Restitution>(hash("Restitution")).
					data<&CircleColider2DComponent::RestitutionThreshold>(hash("RestitutionThreshold")).
					data<&CircleColider2DComponent::Radius>(hash("Radius")).
					data<&CircleColider2DComponent::RuntimeFixture>(hash("RuntimeFixture"));
			}

			{
				//Text2DComponent
				auto factory = entt::meta<Text2DComponent>().type(hash("Text2DComponent"));
				factory.
					data<&Text2DComponent::Text>(hash("Offset")).
					data<&Text2DComponent::font>(hash("RuntimeFixture"));
			}

			s_Init = true;
		}
	}
}