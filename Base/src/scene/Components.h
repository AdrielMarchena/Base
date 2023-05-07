#pragma once

#include "render/gl/Texture.h"
#include "render/SubTexture.h"
#include "SceneCamera.h"
#include "render/Colors.h"
#include "ent/Animator.h"
#include "uuid/UUID.h"
#include "render/Text.h"
#include "misc/PerlinNoise.h"
#include "glm/glm.hpp"
#include <glm/gtx/quaternion.hpp>
#include <typeinfo>
namespace Base {
	template<typename T>
	struct Component
	{
		static inline const std::string StructName = std::string();
		static inline const std::string ComponentName = std::string();
		static inline const std::string RawComponentName = std::string();
		static inline const std::string OnlyTheName = std::string();

		static inline const UUID Uuid = UUID();

		Component()
		{
			const std::string name = typeid(T).name();

			if (Component<T>::RawComponentName == std::string())
			{
				const_cast<std::string&>(Component<T>::RawComponentName) = name;
			}
			if (Component<T>::StructName == std::string())
			{
				const std::string n = name;
				const_cast<std::string&>(Component<T>::StructName) = n.substr(n.find_last_of(':') + 1);
				BASE_CORE_ASSERT(name.find("Component") != std::string::npos, "'{0}' must have the suffix 'Component' to inherit from the struct 'Component<>\n\tExample : 'struct {0}Component : Component<{0}Component> ...''", Component<T>::StructName);

			}
			if (Component<T>::ComponentName == std::string())
			{
				const_cast<std::string&>(Component<T>::ComponentName) = Component<T>::StructName;
				const_cast<std::string&>(Component<T>::ComponentName).insert(Component<T>::StructName.find("Component"), " ");
			}
			if (Component<T>::OnlyTheName == std::string())
			{
				const_cast<std::string&>(Component<T>::OnlyTheName) = Component<T>::StructName;
				const auto str = std::string("Component");
				const auto ind = Component<T>::OnlyTheName.find(str);
				const_cast<std::string&>(Component<T>::OnlyTheName).erase(Component<T>::OnlyTheName.begin() + ind, Component<T>::OnlyTheName.end());
			}
		}
		Component(const Component&) = default;
		// virtual ~Component() = default;

	};

	struct IDComponent : public Component<IDComponent>
	{
		UUID Id;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(UUID uuid)
			:Id(uuid)
		{
		}
	};

	struct TagComponent : public Component<TagComponent>
	{
		std::string Tag = "";

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			:Tag(tag)
		{
		}
	};

	struct TransformComponent : public Component<TransformComponent>
	{

		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent()
			:Translation(glm::vec3(0.0f, 0.0f, 0.0f))
		{
		}
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation)
		{
		}


		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct TextureComponent : public Component<TextureComponent> //TODO: merge with sprite component maybe, no need for 2 components + Serialize
	{
		Ref<render::Texture> Texture;

		TextureComponent() = default;
		TextureComponent(const TextureComponent&) = default;
		TextureComponent(Ref<render::Texture> texture)
			:Texture(texture)
		{
		}

	};

	struct AnimateComponent : public Component<AnimateComponent> //TODO: This probbly don't work anymore, also, remove this namespace ett + Serialize
	{
		ett::Animator Animation;

		AnimateComponent() = default;
		AnimateComponent(const AnimateComponent&) = default;
		AnimateComponent(const ett::Animator& animation)
			:Animation(animation)
		{
		}
	};

	struct SubTextureComponent : public Component<SubTextureComponent> //TODO: Check id this works (I think it still) + Serialize
	{
		render::SubTexture SubTexture;

		SubTextureComponent() = default;
		SubTextureComponent(const SubTextureComponent&) = default;
		SubTextureComponent(const render::SubTexture& sub_texture)
			:SubTexture(sub_texture)
		{
		}

	};

	struct SpriteComponent : public Component<SpriteComponent> //TODO: See TextureComponent todo
	{
		glm::vec4 Color = Color::Base_Color;

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(const glm::vec4& color)
			:Color(color)
		{
		}
	};

	struct CircleComponent : public Component<CircleComponent>
	{
		float Radius = 1.0f;
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleComponent() = default;
		CircleComponent(const CircleComponent&) = default;
		CircleComponent(float radius, float thickness, float fade)
			:Radius(radius), Thickness(thickness), Fade(fade)
		{
		}
	};

	struct CameraComponent : public Component<CameraComponent>
	{
		Base::SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
	class ScriptableEntity;
	struct NativeScriptComponent : public Component<NativeScriptComponent>
	{
		ScriptableEntity* Instance;

		ScriptableEntity* (*InstantiateScript)();
		void(*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* script) { delete script->Instance; script->Instance = nullptr; };
		}
	};

	struct RigidBody2DComponent : public Component<RigidBody2DComponent>
	{
		enum class BodyType
		{
			Static = 0, Dynamic, Kinematic
		};
		BodyType Type = BodyType::Dynamic;

		bool FixedRotation = false;
		void* RuntimeBody = nullptr;

		RigidBody2DComponent() = default;
		RigidBody2DComponent(const RigidBody2DComponent&) = default;
	};

	struct BoxColider2DComponent : public Component<BoxColider2DComponent>
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

	struct CircleColider2DComponent : public Component<CircleColider2DComponent>
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

	struct Text2DComponent : public Component<Text2DComponent>
	{
		std::string Text;
		Ref<Font> font;

		Text2DComponent() = default;
		Text2DComponent(const Text2DComponent&) = default;
	};

	struct Perlin2dComponent : public Component<Perlin2dComponent>
	{
		int octaves = 8;
		float bias = 1.5f;
		float colorInterpolationPrecision = 0.033f;
		glm::vec3 minimumColor = { 75.0f, 75.0f, 75.0f };
		glm::vec3 maxColor = { 225.0f, 225.0f, 225.0f };

		Scope<PerlinNoise2D> Noise;

		Perlin2dComponent() = default;
		// Perlin2dComponent(const Perlin2dComponent&) = default;
	};

	//Temp
	struct Init
	{
		float Test = 0.0f;
		Init() = default;
		Init(const Init&) = default;
	public:
		static void InitComponentsReflection();
		static void InitComponents();
	};
}
