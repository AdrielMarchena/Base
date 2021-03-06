/*
*	@file Ambient.h
*	@author Adriel Marchena Santos
*	
*	Ambient system wich provide a simple way to insert
*	ambient light and source lights in a shader
*/
#pragma once

#include "Base/Base.h"

#include "glm/glm.hpp"
#include "render/gl/Shader.h"
#include "camera/OrthographicCameraController.h"
#include <vector>
#include <iostream>
namespace Base
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
		const static glm::vec3 Middle;

		Ambient():m_Ambient(Day)
		{
		}

		/**
		* Return a default value if the index is <= 0 or > than the
		* number of lights
		*/
		LightSource& operator[](uint32_t index)
		{
			if (index >= 0 && index < m_StaticLightSource.size())
			{
				return m_StaticLightSource[index];
			}
			LightSource blankLightSource{};
			return blankLightSource;
		}
		/**
		* update the Ambient Light
		*/
		void UpdateAmbient(const glm::vec3& value, Ref<render::Shader> _Shader, bool UpdateShader = true);

		void AddStaticLightSource(const LightSource& source);

		// Insert all data in the givin shader
		void UpdateStaticLight(Ref<render::Shader>, const uint8_t _LightSourceLimit = 20);
	
		void ZeroLight(Ref<render::Shader> _Shader);
	};
}	
}



