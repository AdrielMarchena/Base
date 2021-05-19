/*
*	@file OrthographicCameraController.h
*	@author Adriel Marchena Santos
*	
*	Camera Contoller to the camera system
*
*/

#pragma once

#include "OrthographicCamera.h"
#include "args/UpdateArgs.h"

namespace en {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float_t aspectRatio, bool rotation = false);

		void OnUpdate(UpdateArgs args);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float_t GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float_t level) { m_ZoomLevel = level; }
		bool OnMouseScrolled(double_t yoffset, float_t m_value = 0.15f);
		bool Resize(float_t w, float_t h);
	private:
		bool OnWindowResized(float_t w, float_t h);
	private:
		float_t m_AspectRatio;
		float_t m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float_t m_CameraRotation = 0.0f; //In degrees, in the anti-clockwise direction
		float_t m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
	};

}