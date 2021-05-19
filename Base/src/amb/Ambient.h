/*
*	@file Ambient.h
*	@author Adriel Marchena Santos
*	
*	Ambient system wich provide a simple way to insert
*	ambient light and source lights in a shader
*/

#pragma once

#include "glm/glm.hpp"
#include "render/Shader.h"
#include "camera/OrthographicCameraController.h"
#include <vector>
#include <iostream>
#include "utils/Logs.h"
namespace en
{
namespace windowing
{
	struct LightSource
	{
		glm::vec3 u_LightPos;
		glm::vec4 u_LightColor;
		float_t   m_LightIntencity;
	};
	class Ambient
	{
	private:
		std::vector<LightSource> m_StaticLightSource;
		
		glm::vec3 m_Ambient;

	public:
		const static glm::vec3 Night;
		const static glm::vec3 Day;

		Ambient():m_Ambient(Day){}

		LightSource& operator[](uint32_t index)
		{
			if (index >= 0)
			{
				return m_StaticLightSource[index];
			}
		}
		/**
		* update the Ambient Light
		*/
		void UpdateAmbient(glm::vec3 value, const render::Shader& _Shader, bool UpdateShader = true);

		void AddStaticLightSource(const LightSource& source);

		// Insert all data in the givin shader
		void UpdateStaticLight(const render::Shader& _Shader, const uint8_t _LightSourceLimit = 20);
	};
}	
}



