/*
*	@file Shader.cpp
*	
*	Shader Stuff, parser to external files etc...
*
*	Implementation File
*/

#include "Shader.h"

#include "Base/Base.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "gl/glew.h"
#include "utils/gl_error_macro_db.h"

#include "Log.h"

namespace Base
{
namespace render
{

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		return 0;
	}

	Shader::Shader(const std::string& path)
		:m_Path(path)
	{
		std::string source = ReadFile(path);
		m_GL_SourceCode = PreProcess(source);

		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = path.rfind('.');
		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
		m_Name = path.substr(lastSlash,count);

		CreateProgram();
	}

	Shader::Shader(const std::string& name, const std::string& vs, const std::string& fs)
		:m_Name(name)
	{
		m_GL_SourceCode[GL_VERTEX_SHADER] = vs;
		m_GL_SourceCode[GL_FRAGMENT_SHADER] = fs;
		CreateProgram();
	}

	std::string Shader::ReadFile(const std::string& path)
	{
		std::string res;
		std::ifstream in(path, std::ios::in | std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				res.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&res[0], size);
			}
			else
			{ 
				BASE_ERROR("Could not read file {0}", path);
			}
		}
		else
		{
			BASE_ERROR("Could not open file {0}", path);
		}
		return res;
	}

	std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source)
	{

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			BASE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			BASE_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			BASE_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos);

			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}
		return shaderSources;
	}

	void Shader::CreateProgram()
	{
		GLCall(GLuint program = glCreateProgram());

		const char* vsource = m_GL_SourceCode[GL_VERTEX_SHADER].c_str();
		const char* fsource = m_GL_SourceCode[GL_FRAGMENT_SHADER].c_str();

		GLCall(GLuint vs = glCreateShader(GL_VERTEX_SHADER));
		GLCall(glShaderSource(vs, 1, &vsource, NULL));
		GLCall(glCompileShader(vs));

		GLCall(GLuint fs = glCreateShader(GL_FRAGMENT_SHADER));
		GLCall(glShaderSource(fs, 1, &fsource, NULL));
		GLCall(glCompileShader(fs));

		GLCall(glAttachShader(program, vs));
		GLCall(glAttachShader(program, fs));
		GLCall(glLinkProgram(program));

		GLCall(glDetachShader(program, vs));
		GLCall(glDeleteShader(vs));
		GLCall(glDetachShader(program, fs));
		GLCall(glDeleteShader(fs));
		
		m_Id = program;
	}

	Shader::~Shader()
	{
	}

	Ref<Shader> Shader::CreateShader(const std::string& name, const std::string& vs, const std::string& fs)
	{
		Shader new_sh(name,vs, fs);
		return MakeRef<Shader>(new_sh);
	}

	Ref<Shader> Shader::CreateShader(const std::string& path)
	{
		Shader new_sh(path);
		return MakeRef<Shader>(new_sh);
	}

	void Shader::Bind() const
	{
		GLCall(glUseProgram(m_Id));
	}

	void Shader::Unbind() const
	{
		GLCall(glUseProgram(0));
	}

	void Shader::Dispose()
	{
		if(!disposed)
		if (m_Id)
		{
			GLCall(glDeleteProgram(m_Id));
			m_Id = NULL;
			disposed = true;
		}
	}

	void Shader::SetUniform1i(const std::string& name, int32_t value)
	{
		GLCall(GLint location = glGetUniformLocation(m_Id, name.c_str()));
		GLCall(glUniform1i(location, value));
	}

	void Shader::SetUniform1iv(const std::string& name, int32_t size, int32_t* value)
	{
		GLCall(GLint location = glGetUniformLocation(m_Id, name.c_str()));
		GLCall(glUniform1iv(location, size ,value));
	}

	void Shader::SetUniform1f(const std::string& name, float_t value)
	{
		GLCall(GLint location = glGetUniformLocation(m_Id, name.c_str()));
		GLCall(glUniform1f(location, value));
	}

	void Shader::SetUniform2f(const std::string& name, float_t v0, float_t v1)
	{
		GLCall(GLint location = glGetUniformLocation(m_Id, name.c_str()));
		GLCall(glUniform2f(location, v0, v1));
	}

	void Shader::SetUniform3f(const std::string& name, float_t v0, float_t v1, float_t v2)
	{
		GLCall(GLint location = glGetUniformLocation(m_Id, name.c_str()));
		GLCall(glUniform3f(location, v0, v1, v2));
	}

	void Shader::SetUniform4f(const std::string& name, float_t v0, float_t v1, float_t v2, float_t v3)
	{
		GLCall(GLint location = glGetUniformLocation(m_Id, name.c_str()));
		GLCall(glUniform4f(location, v0, v1, v2, v3));
	}

	void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
	{
		GLCall(GLint location = glGetUniformLocation(m_Id, name.c_str()));
		GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
	}

	//----------------------------------------------------------------------------------------------------------------

	void ShaderLib::Add(const std::string& name, const Ref<Shader>& shader)
	{
		BASE_CORE_ASSERT(!Exists(name), "Shader already exists");
		m_Shaders[name] = shader;
	}

	void ShaderLib::Add(const Ref<Shader>& shader)
	{
		BASE_CORE_ASSERT(!Exists(shader->GetName()), "Shader already exists");
		m_Shaders[shader->GetName()] = shader;
	}

	Ref<Shader> ShaderLib::Load(const std::string& path)
	{
		auto shader = Shader::CreateShader(path);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLib::Load(const std::string& name, const std::string& path)
	{
		auto shader = Shader::CreateShader(path);
		Add(name,shader);
		return shader;
	}

	Ref<Shader> ShaderLib::Get(const std::string& name)
	{
		BASE_CORE_ASSERT(Exists(name), "Shader does not exist");
		return m_Shaders[name];
	}

	bool ShaderLib::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}
}