/*
* @file Files.h
* @author Adriel Marchena Santos
*
* File handlind things here
*/

#pragma once

#include <vector>
#include <string>

namespace en
{
namespace utils
{

	namespace ex
	{
		/**
		* Exception used when a directory is not found
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
			const std::string& path() const { return _Path; }
		private:
			std::string _Path;
		};
	}

	class Files
	{
	public:
		Files(){}

		static std::vector<std::pair<std::string, std::string>> GetPairText(const std::string& dir, const std::string& ext = ".jpg#.png#.jpeg");
	};
}
}

