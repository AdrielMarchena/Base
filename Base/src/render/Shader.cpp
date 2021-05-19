#include "Shader.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
namespace en
{
namespace render
{
	Shader::Shader(const char* vs, const char* fs, int32_t MaxTexSlots)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vs);
			fShaderFile.open(fs);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();

			std::string line = "";
			while (getline(fShaderStream, line))
			{
				if (line.find("#define MAX_TEXTURES_SLOTS") != std::string::npos)
				{
					line.append(" " + std::to_string(MaxTexSlots));
				}
				fragmentCode.append(line + '\n');
			}
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ, ERROR: " << e.what() << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		assert(vertex > 0);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		assert(fragment > 0);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// shader Program
		m_Id = glCreateProgram();
		assert(m_Id > 0);
		glAttachShader(m_Id, vertex);
		glAttachShader(m_Id, fragment);
		glLinkProgram(m_Id);
		checkCompileErrors(m_Id, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	Shader::~Shader()
	{
		if (m_Id && !copy)
		{
			glDeleteProgram(m_Id);
			m_Id = NULL;
		}
	}

	void Shader::Bind() const
	{
		glUseProgram(m_Id);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetUniform1i(const std::string& name, int32_t value) const
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void Shader::SetUniform1iv(const std::string& name, int32_t size, int32_t* value) const
	{
		glUniform1iv(GetUniformLocation(name), size ,value);
	}

	void Shader::SetUniform1f(const std::string& name, float_t value) const
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void Shader::SetUniform2f(const std::string& name, float_t v0, float_t v1) const
	{
		glUniform2f(GetUniformLocation(name), v0, v1);
	}

	void Shader::SetUniform3f(const std::string& name, float_t v0, float_t v1, float_t v2) const
	{
		glUniform3f(GetUniformLocation(name), v0, v1, v2);
	}

	void Shader::SetUniform4f(const std::string& name, float_t v0, float_t v1, float_t v2, float_t v3) const
	{
		glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
	}

	void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}

	int32_t Shader::GetUniformLocation(const std::string& name) const
	{
		if (m_Locations.find(name) != m_Locations.end())
			return m_Locations[name];

		int32_t location = glGetUniformLocation(m_Id, name.c_str());
		if(location == -1)
			std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
		m_Locations[name] = location;
		return location;
	}


	void Shader::checkCompileErrors(uint32_t shader, const char* type)
	{
		int32_t success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}

	std::string Shader::GetShaderFile(const char* path, int32_t MaxTexSlots)
	{
		std::string line;
		std::ifstream stream(path);
		std::stringstream final_stream;
		while (getline(stream, line))
		{
			if (line.find("#define MAX_TEXTURES_SLOTS") != std::string::npos)
			{
				line.append(" " + std::to_string(MaxTexSlots));
			}
			final_stream << line << '\n';
		}
		return final_stream.str();
	}
}
}