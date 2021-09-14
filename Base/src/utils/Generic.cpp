#include "Generic.h"

namespace Base
{
namespace utils
{
	std::string ToUpper(const std::string& str)
	{
		std::string tmp = "";
		for (auto& c : str)
		{
			if (c >= 'a' && c <= 'z')
			{
				tmp += c - 32;
			}
			else
				tmp += c;
		}
		return tmp;
	}
	std::string ToLower(const std::string& str)
	{
		std::string tmp = "";
		for (auto& c : str)
		{
			if (c >= 'A' && c <= 'Z')
			{
				tmp += c + 32;
			}
			else
				tmp += c;
		}
		return tmp;
	}
}
}