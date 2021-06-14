#pragma once

#include <exception>

namespace en
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
}
}
}