#pragma once

#include "glm/glm.hpp"
#include "render/Texture.h"
#include "render/SubTexture.h"
#include "glm/glm.hpp"
#include "render/Camera.h"
namespace en
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

	struct SpriteComponent
	{
		render::Texture Texture;
		render::SubTexture SubTexture;
		float_t Layer = 0.0f;
		glm::vec4 Color = {1.0f,1.0f,1.0f,1.0f};
		float_t Rotation = 0.0f;
		glm::vec3 Axis = { 0.0f,0.0f,1.0f };

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(const glm::vec4 color)
			:Color(color) {}
		SpriteComponent(render::Texture texture,const glm::vec4& color)
			:Texture(texture), Color(color){}
		SpriteComponent(render::SubTexture sub_texture, const glm::vec4& color)
			:SubTexture(sub_texture), Color(color) {}
	};

	struct SquareColisionComponent // Use the Position and size
	{
		bool Active = true;
		bool Checking = false;
	};

	struct CameraComponent
	{
		en::Camera Camera;
		bool Primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection)
			:Camera(projection) {}
	};
}
