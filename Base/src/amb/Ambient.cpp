/*
*	@file Ambient.cpp
*	@author Adriel Marchena Santos
*	
*	Ambient system wich provide a simple way to insert
*	ambient light and source lights in a shader
*	
*	Implementation File
*/
#include "pch.h"
#include "Ambient.h"
#include "Log.h"
namespace Base
{
namespace windowing
{

	//Static members Init
	const glm::vec3 Ambient::Night = { 0.1f,0.1f,0.1f };
	const glm::vec3 Ambient::Day = { 1.0f,1.0f,1.0f };
	const glm::vec3 Ambient::Middle = { 0.5f,0.5f,0.5f };

	void Ambient::UpdateAmbient(const glm::vec3& value, Ref<render::Shader> _Shader, bool UpdateShader)
	{
		m_Ambient = value;
		if (UpdateShader)
			_Shader->SetUniform3f("u_Ambient", m_Ambient.x, m_Ambient.y, m_Ambient.z);
	}

	void Ambient::AddStaticLightSource(const LightSource& source)
	{
		m_StaticLightSource.push_back(source);
		m_StaticLightSource.shrink_to_fit(); //Keep size consistent to the shader
	}

	void Ambient::UpdateStaticLight(Ref<render::Shader> _Shader, const uint8_t _LightSourceLimit)
	{
		uint8_t l_size = std::min(m_StaticLightSource.size(), (std::size_t)_LightSourceLimit);

		_Shader->Bind();
		_Shader->SetUniform1f("u_LightQtd", l_size);
		_Shader->SetUniform3f("u_Ambient", m_Ambient.x, m_Ambient.y, m_Ambient.z);
		std::string norm_name = "";
		for (int i = 0; i < l_size; i++)
		{
			norm_name = "u_LightInfo[" + std::to_string(i) + "]";
			_Shader->SetUniform3f(norm_name + ".u_LightPos", m_StaticLightSource[i].u_LightPos.x,
				m_StaticLightSource[i].u_LightPos.y,
				m_StaticLightSource[i].u_LightPos.z);

			_Shader->SetUniform4f(norm_name + ".u_LightColor", m_StaticLightSource[i].u_LightColor.x,
				m_StaticLightSource[i].u_LightColor.y,
				m_StaticLightSource[i].u_LightColor.z,
				m_StaticLightSource[i].u_LightColor.w);

			_Shader->SetUniform1f(norm_name + ".u_LightIntencity", m_StaticLightSource[i].m_LightIntencity);
			BASE_DEBUG_LOG("Static light updated");
		}
	}

	void Ambient::ZeroLight(Ref<render::Shader> _Shader)
	{
		m_StaticLightSource.clear();
		_Shader->SetUniform1f("u_LightQtd", 0);
	}

}
}