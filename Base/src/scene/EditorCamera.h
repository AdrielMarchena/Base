#pragma once
#include "args/UpdateArgs.h"
#include "event/MouseEvent.h"

#include "render/Camera.h"

#include "glm/glm.hpp"
#include <glm/gtx/quaternion.hpp>
#include <utility>
namespace Base
{
	class EditorCamera : public Camera
	{
	private:
		float m_ViewportWidth = 800;
		float m_ViewportHeight = 600;
		float m_Distance = 10.0f;
		float m_Pitch = 0.0f;
		float m_Yaw = 0.0f;
		float m_FOV = 45.0f;
		float m_AspectRatio = 1.778f;
		float m_NearClip = 0.1f;
		float m_FarClip = 1000.0f;

		glm::vec3 m_Position = {0.0f,0.0f,0.0f};
		glm::vec3 m_FocalPoint = {0.0f,0.0f, 0.0f};
		glm::mat4 m_ViewMatrix;

		glm::vec2 m_InitialMousePosition;

	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(const UpdateArgs& args);
		
		inline float GetDistance() const { return m_Distance; }
		inline void GetDistance(float distance) { m_Distance = distance; }

		void SetViewportSize(float width, float height) {
			m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection();
		}

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }
		void OnMouseScroll(MouseScrollEvent& e);
	private:
		void UpdateProjection();
		void UpdateView();


		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	};
}