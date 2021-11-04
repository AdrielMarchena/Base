#pragma once

#include "glm/glm.hpp"
#include "render/gl/Texture.h"
#include "render/SubTexture.h"
#include "glm/glm.hpp"
#include <glm/gtx/quaternion.hpp>
#include "SceneCamera.h"
#include "render/Colors.h"
#include "ent/Animator.h"
#include "ScriptableEntity.h"
#include "render/3d/Model.h"
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
		
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation =	{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale =		{ 1.0f, 1.0f, 1.0f };
		//TODO: Maybe Temp
		glm::mat4 Manual_Transform = glm::mat4(0.0f);

		TransformComponent()
			:Translation(glm::vec3(0.0f, 0.0f, 0.0f)), Manual_Transform(glm::mat4(0.0f)) {}
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation), Manual_Transform(glm::mat4(0.0f)) {}


		glm::mat4 GetTransform() const
		{
			if (Manual_Transform != glm::mat4(0.0f))
				return Manual_Transform;

			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
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
		float Radius = 1.0f;
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleComponent() = default;
		CircleComponent(const CircleComponent&) = default;
		CircleComponent(float radius, float thickness,float fade)
			:Radius(radius), Thickness(thickness), Fade(fade) {}
	};

	struct SquareColisionComponent // Use the Position and size
	{
		bool Active = true;
		bool Checking = false;
	};

	struct ModelComponent
	{
		Ref<Model> Model3D;

		ModelComponent() = default;
		ModelComponent(const ModelComponent&) = default;
		ModelComponent(Ref<Model> model)
			:Model3D(model) {}
	};

	struct CameraComponent
	{
		Base::SceneCamera Camera;
		bool Primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
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