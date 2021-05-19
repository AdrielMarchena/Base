#include "OrthographicCameraController.h"
#include "input/Keyboard.h"
namespace en {

	OrthographicCameraController::OrthographicCameraController(float_t aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(UpdateArgs args)
	{
		auto& keyboard = args.keyboard;
		if (keyboard.isPress(GLFW_KEY_LEFT))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * args.dt;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * args.dt;
		}
		else if (keyboard.isPress(GLFW_KEY_RIGHT))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * args.dt;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * args.dt;
		}

		if (keyboard.isPress(GLFW_KEY_UP))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * args.dt;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation))  * m_CameraTranslationSpeed *  args.dt;
		}
		else if (keyboard.isPress(GLFW_KEY_DOWN))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * args.dt;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation))  * m_CameraTranslationSpeed *  args.dt;
		}

		if (m_Rotation)
		{
			if (keyboard.isPress(GLFW_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * args.dt;
			if (keyboard.isPress(GLFW_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * args.dt;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	bool OrthographicCameraController::OnMouseScrolled(double_t yoffset, float_t m_value)
	{
		m_ZoomLevel -= yoffset * m_value;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::Resize(float_t w, float_t h)
	{
		m_AspectRatio = w / h;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(float_t w, float_t h)
	{
		m_AspectRatio = w / h;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}