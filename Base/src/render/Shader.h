/*
*	@file Shader.h
*	
*	Shader Stuff, parser to external files etc...
*/

#pragma once
#include <string>
#include <cstdint>
#include <unordered_map>
#include "glm/glm.hpp"
#include "gl/glew.h"
#include "utils/gl_error_macro_db.h"

namespace en
{
namespace render
{
	class Shader
	{
	private:
		uint32_t m_Id = NULL;
		mutable std::unordered_map<std::string, int32_t> m_Locations;
		bool copy = false;
		bool disposed = false;
	public:

		Shader() = default;
		Shader(const char* vs, const char* fs,int32_t MaxTexSlots = 8);
		~Shader();

		Shader(Shader& other) = delete;

		Shader& operator=(Shader& other) = delete;

		Shader& operator=(Shader&& other) noexcept
		{
			if (this == &other)
				return *this;

			if(m_Id)
				GLCall(glDeleteProgram(m_Id));
			m_Locations.clear();

			m_Id = other.m_Id;
			m_Locations = other.m_Locations;

			other.m_Id = NULL;
			other.m_Locations.clear();

			return *this;
		}

		void Bind() const;
		void Unbind() const;

		void Dispose();

		void SetUniform1i(const std::string& name, int32_t value) const;
		void SetUniform1iv(const std::string& name, int32_t size, int32_t* value) const;
		void SetUniform1f(const std::string& name, float_t value) const;
		void SetUniform2f(const std::string& name, float_t v0, float_t v1) const;
		void SetUniform3f(const std::string& name, float_t v0, float_t v1, float_t v2) const;
		void SetUniform4f(const std::string& name, float_t v0, float_t v1, float_t v2, float_t v3) const;
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const;

		int32_t GetUniformLocation(const std::string& name) const;

	private:
		void checkCompileErrors(uint32_t shader, const char* type);
		std::string GetShaderFile(const char* path, int32_t MaxTexSlots = 8);
	};
}
}