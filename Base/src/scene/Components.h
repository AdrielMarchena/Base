#pragma once

#include "glm/glm.hpp"
#include "render/Texture.h"
#include "glm/glm.hpp"
namespace en
{

	struct PositionComponent
	{
		glm::vec3 Position = { 0.0f,0.0f,0.0f };
		glm::vec2 Size = { 1.0f,1.0f };

		PositionComponent() = default;
		PositionComponent(const PositionComponent&) = default;
		PositionComponent(const glm::vec3& position,const glm::vec2& size)
			:Position(position),Size(size){}
	};

	struct SpriteComponent
	{
		std::shared_ptr<render::Texture> Texture;
		glm::vec4 Color = {1.0f,1.0f,1.0f,1.0f};

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(std::shared_ptr<render::Texture> texture,const glm::vec4 color)
			:Texture(texture), Color(color){}
	};
}
