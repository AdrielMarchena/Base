#pragma once

#include "glm/glm.hpp"

namespace en
{
	struct SpriteComponent
	{
		glm::vec3 position;
		glm::vec2 size;
		
		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(const glm::vec3& position, const glm::vec2& size)
			:position(position),size(size){ }

	};
}
