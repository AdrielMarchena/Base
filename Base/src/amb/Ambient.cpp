/*
*	@file Ambient.cpp
*	@author Adriel Marchena Santos
*	
*	Ambient system wich provide a simple way to insert
*	ambient light and source lights in a shader
*	
*	Implementation File
*/

#include "Ambient.h"

namespace en
{
namespace windowing
{

	//Static members Init
	const glm::vec3 Ambient::Night = { 30.0f,30.0f,30.0f };
	const glm::vec3 Ambient::Day = { 1000.0f,1000.0f,1000.0f };

	void Ambient::UpdateAmbient(glm::vec3 value, const render::Shader& _Shader, bool UpdateShader)
	{
		//TODO: maybe add some validation to value here
		m_Ambient = value;
		if (UpdateShader)
			_Shader.SetUniform3f("u_Ambient", m_Ambient.x, m_Ambient.y, m_Ambient.z);
	}

	void Ambient::AddStaticLightSource(const LightSource& source)
	{
		m_StaticLightSource.push_back(source);
		m_StaticLightSource.shrink_to_fit(); //Keep size consistent to the shader
	}

	void Ambient::UpdateStaticLight(const render::Shader& _Shader, const uint8_t _LightSourceLimit)
	{
		uint8_t l_size = std::min(m_StaticLightSource.size(), (std::size_t)_LightSourceLimit);

		_Shader.Bind();
		_Shader.SetUniform1f("u_LightQtd", l_size);
		_Shader.SetUniform3f("u_Ambient", m_Ambient.x, m_Ambient.y, m_Ambient.z);
		std::string norm_name = "";
		for (int i = 0; i < l_size; i++)
		{
			norm_name = "u_LightInfo[" + std::to_string(i) + "]";
			_Shader.SetUniform3f(norm_name + ".u_LightPos", m_StaticLightSource[i].u_LightPos.x,
				m_StaticLightSource[i].u_LightPos.y,
				m_StaticLightSource[i].u_LightPos.z);

			_Shader.SetUniform4f(norm_name + ".u_LightColor", m_StaticLightSource[i].u_LightColor.x,
				m_StaticLightSource[i].u_LightColor.y,
				m_StaticLightSource[i].u_LightColor.z,
				m_StaticLightSource[i].u_LightColor.w);

			_Shader.SetUniform1f(norm_name + ".u_LightIntencity", m_StaticLightSource[i].m_LightIntencity);
		}
	}

	void Ambient::ZeroLight(const render::Shader& _Shader)
	{
		m_StaticLightSource.clear();
		_Shader.SetUniform1f("u_LightQtd", 0);
	}

}
}