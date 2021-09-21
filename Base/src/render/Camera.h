#pragma once
#include "glm/glm.hpp"

namespace Base
{
	class Camera {
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			:m_Projection(projection){}

		virtual ~Camera(){}

		const glm::mat4 GetProjection() const { return m_Projection; }

	protected:
		glm::mat4 m_Projection;
	};
}