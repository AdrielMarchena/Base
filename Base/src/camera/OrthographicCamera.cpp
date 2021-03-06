/*
*	@file OrthographicCamera.cpp
*	@author Adriel Marchena Santos
*	
*	Camera System
*
*	Implementation File
*/
#include "pch.h"
#include "OrthographicCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Base {

	OrthographicCamera::OrthographicCamera(float_t left, float_t right, float_t bottom, float_t top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetProjection(float_t left, float_t right, float_t bottom, float_t top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		RecalculateViewMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}