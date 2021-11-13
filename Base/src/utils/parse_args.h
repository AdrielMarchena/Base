#pragma once

#include <string>
#include <unordered_map>
#include "utils/Generic.h"
#include "utils/base_assert.h"
namespace Base
{
/* All keys are lower case */
class ConsoleArgsParser
{
private:
	std::string m_PathArg;
	std::unordered_map<std::string, bool> m_Flags;
	std::unordered_map<std::string, std::string> m_Options;
	bool m_Parsed = false;
	bool m_RegistryMode;
public:
	virtual ~ConsoleArgsParser() = default;
	ConsoleArgsParser(int argc, char* argv[])
		:m_RegistryMode(false)
	{
		if (argc)
			m_PathArg = argv[0];
		ParseArgs(argc, argv);
	}

	ConsoleArgsParser()
		:m_RegistryMode(true)
	{
	}

	void ParseArgs(int argc, char* argv[]) 
	{
		if (m_Parsed)
			return;
		P_ParseArgs(argc, argv);
		m_Parsed = true;
	}

	bool GetFlag(const std::string& flag) const;

	void RegistryFlag(const std::string& flag_name);
	void RegistryOption(const std::string& opt_name);

	float GetOptionAsFloat(const std::string& opt) const;
	int GetOptionAsInt(const std::string& opt) const;
	const std::string& GetOptionAsString(const std::string& opt) const;
	const std::string& GetOption(const std::string& opt) const;

	const std::string& GetArgZero() const { return m_PathArg; }
	const std::unordered_map<std::string, std::string>& GetOptMap() const { return m_Options; }
	const std::unordered_map<std::string, bool>& GetFlagMap() const { return m_Flags; }
private:
	void P_ParseArgs(int argc, char* argv[]);
};

}
