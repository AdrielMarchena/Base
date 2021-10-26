#pragma once

#include <string>
#include <unordered_map>
#include "utils/Generic.h"
#include "utils/base_assert.h"
namespace Base
{
namespace utils
{
	/* All keys are lower case */
	class ConsoleArgsParser
	{
	private:
		std::unordered_map<std::string, bool> m_Flags;
		std::unordered_map<std::string, std::string> m_Options;
	public:
		virtual ~ConsoleArgsParser() = default;
		ConsoleArgsParser(int argc, char* argv[])
		{
			ParseArgs(argc, argv);
		}

	public:
		bool GetFlag(const std::string& flag) const;

		float GetOptionAsFloat(const std::string& opt) const;
		int GetOptionAsInt(const std::string& opt) const;
		const std::string& GetOptionAsString(const std::string& opt) const;
		const std::string& GetOption(const std::string& opt) const;


	protected:
		void ParseArgs(int argc, char* argv[]);
	};
}
}
