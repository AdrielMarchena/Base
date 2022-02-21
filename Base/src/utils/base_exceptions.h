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
		directory_not_found() noexcept
		{
		}

		directory_not_found(const char* what) noexcept
		{
			_What = _What;
		}

		directory_not_found(const char* what, const char* path) noexcept
		{
			_What = what;
			_Path = path;
		}

		virtual char const* path() const noexcept
		{
			return _Path.c_str();
		}
		virtual const char* what() const noexcept override
		{
			return _What.c_str();
		}
	private:
		std::string _Path = "Unknown path";
		std::string _What = "";
	};

	class WindowCreationException : public std::exception
	{
	public:
		enum class Reasons : uint8_t
		{
			NOT_SPECIFIED = 0,
			GLFW3,
			GLEW,
			GLAD
		};
		WindowCreationException() noexcept
		{ 
			_What = "";
			_Reason = Reasons::NOT_SPECIFIED;
		}
		WindowCreationException(const char* what) noexcept
		{ 
			_What = what;
			_Reason = Reasons::NOT_SPECIFIED;
		}
		WindowCreationException(const char* what,Reasons reason) noexcept
		{ 
			_What = what;
			_Reason = reason;
		}

		Reasons reason() const noexcept { return _Reason; }
		virtual const char* what() const noexcept override
		{
			return _What.c_str();
		}
	private:
		Reasons _Reason;
		std::string _What = "";
	};
}
}
}