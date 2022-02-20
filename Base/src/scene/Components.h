#pragma once

#include "render/gl/Texture.h"
#include "render/SubTexture.h"
#include "SceneCamera.h"
#include "render/Colors.h"
#include "ent/Animator.h"
#include "uuid/UUID.h"
#include "render/Text.h"
#include "glm/glm.hpp"
#include <glm/gtx/quaternion.hpp>
namespace Base
{
	struct IDComponent
	{
		UUID Id;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(UUID uuid)
			:Id(uuid){}
	};

	struct TagComponent
	{
		std::string Tag = "";

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			:Tag(tag) {}
	};

	struct TransformComponent
	{
		
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation =	{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale =		{ 1.0f, 1.0f, 1.0f };

		TransformComponent()
			:Translation(glm::vec3(0.0f, 0.0f, 0.0f)) {}
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}


		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct TextureComponent //TODO: merge with sprite component maybe, no need for 2 components + Serialize
	{
		Ref<render::Texture> Texture;

		TextureComponent() = default;
		TextureComponent(const TextureComponent&) = default;
		TextureComponent(Ref<render::Texture> texture)
			:Texture(texture) {}

	};

	struct AnimateComponent //TODO: This probbly don't work anymore, also, remove this namespace ett + Serialize
	{
		ett::Animator Animation;

		AnimateComponent() = default;
		AnimateComponent(const AnimateComponent&) = default;
		AnimateComponent(const ett::Animator& animation)
			:Animation(animation) {}
	};

	struct SubTextureComponent //TODO: Check id this works (I think it still) + Serialize
	{
		render::SubTexture SubTexture;

		SubTextureComponent() = default;
		SubTextureComponent(const SubTextureComponent&) = default;
		SubTextureComponent(const render::SubTexture& sub_texture)
			:SubTexture(sub_texture) {}

	};

	struct SpriteComponent //TODO: See TextureComponent todo
	{
		glm::vec4 Color = Color::Base_Color;

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(const glm::vec4& color)
			:Color(color) {}
	};

	struct CircleComponent
	{
		float Radius = 1.0f;
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleComponent() = default;
		CircleComponent(const CircleComponent&) = default;
		CircleComponent(float radius, float thickness,float fade)
			:Radius(radius), Thickness(thickness), Fade(fade) {}
	};

	struct CameraComponent
	{
		Base::SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
	class ScriptableEntity;
	struct NativeScriptComponent
	{
		ScriptableEntity* Instance;

		ScriptableEntity*(*InstantiateScript)();
		void(*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* script) { delete script->Instance; script->Instance = nullptr;  };
		}
	};

	struct RigidBody2DComponent
	{
		enum class BodyType
		{
			Static = 0, Dynamic, Kinematic
		};
		BodyType Type = BodyType::Static;

		bool FixedRotation = false;
		void* RuntimeBody = nullptr;

		RigidBody2DComponent() = default;
		RigidBody2DComponent(const RigidBody2DComponent&) = default;
	};

	struct BoxColider2DComponent
	{
		glm::vec2 Offset = { 0.0f,0.0f };
		glm::vec2 Size = { 0.5f,0.5f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		void* RuntimeFixture = nullptr;

		BoxColider2DComponent() = default;
		BoxColider2DComponent(const BoxColider2DComponent&) = default;
	};

	struct CircleColider2DComponent
	{
		glm::vec2 Offset = { 0.0f,0.0f };
		glm::vec2 Size = { 0.5f,0.5f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;
		float Radius = 0.5f;

		void* RuntimeFixture = nullptr;

		CircleColider2DComponent() = default;
		CircleColider2DComponent(const CircleColider2DComponent&) = default;
	};

	struct Text2DComponent
	{
		std::string Text;
		Ref<Font> font;

		Text2DComponent() = default;
		Text2DComponent(const Text2DComponent&) = default;
	};

	//Temp
	class Init
	{
	public:
		static void InitComponentsReflection();
	};
}
