#pragma once

#include <exception>

namespace Base
{
namespace utils
{
namespace baseException
{
	/**
	* Exception throw when a directory is not found
	*/
	class directory_not_found : public std::exception
	{
	public:
		directory_not_found()
			:exception(), _Path("")
		{}
		directory_not_found(const char* _What)
			:exception(_What), _Path("")
		{}
		directory_not_found(const char* _What, const char* _Path)
			:exception(_What), _Path(_Path)
		{}

		_NODISCARD virtual char const* path() const
		{
			return _Path ? _Path : "Unknown path";
		}
	private:
		const char* _Path;
	};

	/**
	* Exception throw when a stb_image return no SOI error
	*/
	class no_soi : public std::exception
	{
	public:
		no_soi()
			:exception(), _Path("")
		{}
		no_soi(const char* _What)
			:exception(_What), _Path("")
		{}
		no_soi(const char* _What, const char* _Path)
			:exception(_What), _Path(_Path)
		{}

		_NODISCARD virtual char const* path() const
		{
			return _Path ? _Path : "Unknown path";
		}
	private:
		const char* _Path;
	};

	class WindowCreationException : public std::exception
	{
	public:
		enum class Reasons : uint8_t
		{
			NOT_SPECIFIED = 0,
			GLFW3,
			GLEW,
		};
		WindowCreationException()
			:exception(), _Reason(Reasons::NOT_SPECIFIED) {}
		WindowCreationException(const char* _What)
			:exception(_What),_Reason(Reasons::NOT_SPECIFIED){}
		WindowCreationException(const char* _What,Reasons reason)
			:exception(_What), _Reason(reason) {}

		Reasons reason() const { return _Reason; }
	private:
		Reasons _Reason;
	};
}
}
}