#pragma once

#include "glm/glm.hpp"
#include "render/Texture.h"
#include "render/SubTexture.h"
#include "glm/glm.hpp"
#include "render/Camera.h"
#include "render/Colors.h"
#include "ent/Animator.h"

#include "ScriptableEntity.h"
namespace Base
{
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
		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			:Transform(transform){}
	};

	struct VelocityComponent
	{
		glm::vec3 Velocity = glm::vec3(0.0f);

		VelocityComponent() = default;
		VelocityComponent(const VelocityComponent&) = default;
		VelocityComponent(const glm::vec3& velocity)
			:Velocity(velocity) {}
	};

	struct TextureComponent
	{
		Ref<render::Texture> Texture;
		float_t Rotation = 0.0f;
		glm::vec3 Axis = { 0.0f,0.0f,1.0f };

		TextureComponent() = default;
		TextureComponent(const TextureComponent&) = default;
		TextureComponent(Ref<render::Texture> texture)
			:Texture(texture) {}

	};

	struct AnimateComponent
	{
		ett::Animator Animation;
		float_t Rotation = 0.0f;
		glm::vec3 Axis = { 0.0f,0.0f,1.0f };

		AnimateComponent() = default;
		AnimateComponent(const AnimateComponent&) = default;
		AnimateComponent(const ett::Animator& animation)
			:Animation(animation) {}
	};

	struct SubTextureComponent
	{
		render::SubTexture SubTexture;
		float_t Rotation = 0.0f;
		glm::vec3 Axis = { 0.0f,0.0f,1.0f };

		SubTextureComponent() = default;
		SubTextureComponent(const SubTextureComponent&) = default;
		SubTextureComponent(const render::SubTexture& sub_texture)
			:SubTexture(sub_texture) {}

	};

	struct SpriteComponent
	{
		float_t Layer = 0.0f;
		glm::vec4 Color = Color::Base_Color;
		float_t Rotation = 0.0f;
		glm::vec3 Axis = { 0.0f,0.0f,1.0f };

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(const glm::vec4& color)
			:Color(color) {}
	};

	struct CircleComponent
	{
		glm::vec3 Position = { 0.0f,0.0f,0.0f }; //Temp
		float Radius = 5.0f;
		glm::vec4 Color = Color::Base_Color;
		bool Fill = true;
		CircleComponent() = default;
		CircleComponent(const CircleComponent&) = default;
		CircleComponent(const glm::vec3& position,float radius, const glm::vec4& color, bool fill = true)
			:Position(position), Radius(radius), Color(color), Fill(fill) {}
	};

	struct SquareColisionComponent // Use the Position and size
	{
		bool Active = true;
		bool Checking = false;
	};

	struct CameraComponent
	{
		Base::Camera Camera;
		bool Primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection)
			:Camera(projection) {}
	};

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
}
