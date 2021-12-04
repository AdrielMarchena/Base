/*
* @file Files.cpp
*
* File handlind things here
*/

/*
* 	@file Files.cpp
* 	@author Adriel Marchena Santos
*
* 	File handlind things here
*
* 	Implementation File
*/

#include "pch.h"
#include "Files.h"

#include <sstream>
#include <iostream>
#include <Windows.h>
#include <filesystem>
namespace Base
{
namespace utils
{
	/**
	* Parse a string using a split char (default is '#')
	*/
	static inline std::vector<std::string> ParseThing(const std::string& thing, char split_char = '#')
	{
		std::vector<std::string> parse_ext;
		//Split extentions
		std::stringstream ext_stream(thing);
		std::string segment;
		while (std::getline(ext_stream, segment, split_char))
		{
			parse_ext.push_back(segment);
		}
		parse_ext.shrink_to_fit();
		return parse_ext;
	}
	/**
	* Receive a directory and the extensions to search on this directory
	* Return a vector of pairs name-path
	*	name = name of the file (without the extension)
	*	path = the full path to the file
	* 
	*	Note: the extension parameter is defaulted to ".jpg#.png#.jpeg"
	*		  '#' character is used to parse the extensions easily
	* 
	*	Throw: An std::filesystem::filesystem_error if the directory does not exist
	*		   You can define 'BASE_ENGINE_NO_FILE_EXEPTION' so this funtions dosent throw that
	*/
	std::vector<std::pair<std::string, std::string>> Files::GetPairText(const std::string& dir, const std::string& ext)
	{
		
		auto parse_ext = ParseThing(ext);
#ifndef BASE_ENGINE_NO_FILE_EXEPTION
		if (!std::filesystem::exists(dir))
		{
			
			const std::string msg = "The directory " + dir + " does not exists"; //FIXME: Do i really need this temp std::string ?
			throw baseException::directory_not_found(msg.c_str(),dir.c_str());
		}
#else
		if (!fs::exists(dir))
		{
			std::vector<std::pair<std::string, std::string>> name_path_pair;
			return name_path_pair;
		}
#endif
		//Grab files path from dir, and parse in a pair name-path
		std::vector<std::pair<std::string, std::string>> name_path_pair;
		for (const auto& entry : std::filesystem::directory_iterator(dir))
		{
			if(!entry.path().empty())
				for (auto& exts : parse_ext)
				{
					if (entry.path().extension().string() == exts)
					{
						std::string name = entry.path().filename().string();
						size_t find = name.find('.');
						name.erase(name.begin()+find,name.end());

						name_path_pair.emplace_back(
							std::make_pair(name, entry.path().string()
						));
					}
				}
		}
		name_path_pair.shrink_to_fit();
		return name_path_pair;
	}
}
}