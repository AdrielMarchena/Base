#include "parse_args.h"

namespace Base
{
namespace utils
{
	float ConsoleArgsParser::GetOptionAsFloat(const std::string& opt) const
	{
		const std::string o = GetOptionAsString(opt);
		if (!o.empty())
		{
			return std::stof(o);
		}
		return -1.0f;
	}

	int ConsoleArgsParser::GetOptionAsInt(const std::string& opt) const
	{
		const std::string o = GetOptionAsString(opt);
		if (!o.empty())
		{
			return std::stoi(o);
		}
		return -1;
	}

	const std::string& ConsoleArgsParser::GetOptionAsString(const std::string& opt) const
	{
		if (!opt.empty())
		{
			auto optionIt = m_Options.find(utils::ToLower(opt));
			if (optionIt != m_Options.end())
			{
				return optionIt->second;
			}
		}
		static std::string EmptyOption = "";
		return EmptyOption;
	}

	const std::string& ConsoleArgsParser::GetOption(const std::string& opt) const
	{
		return GetOptionAsString(opt);
	}

	bool ConsoleArgsParser::GetFlag(const std::string& flag) const
	{
		if (!flag.empty())
		{
			auto flagIt = m_Flags.find(flag);
			if (flagIt != std::end(m_Flags))
			{
				return flagIt->second;
			}
		}
		return false;
	}

	void ConsoleArgsParser::ParseArgs(int argc, char* argv[])
	{
		if (argc > 1)
		{
			for (int i = 0; i < argc; ++i)
			{
				std::string arg = argv[i];
				if (arg.length() >= 3)
				{
					if (arg[0] == '-' && arg[1] == '-')
					{
						arg = arg.substr(2);

						if (arg.find_first_of('=') != std::string::npos)
						{
							//Option
							const std::size_t equalSignPos = arg.find('=');
							if (equalSignPos != std::string::npos)
							{
								std::string optionName = arg.substr(0, equalSignPos);
								std::string optionValue = arg.substr(equalSignPos + 1);

								//Filter by Options Registred
								/*auto optionIt = m_Options.find(optionName);
								if (optionIt != m_Options.end())
								{
									optionIt->second = optionValue;
								}*/

								//Registry everything
								m_Options[utils::ToLower(optionName)] = optionValue;
							}
						}
						else
						{
							//Filter by Flagds Registred
							/*auto flagIt = m_Flags.find(arg);
							if (flagIt != std::end(m_Flags))
							{
								flagIt->second = true;
							}*/

							//Registry everything
							m_Flags[utils::ToLower(arg)] = true;
						}
					}
				}
			}
		}
	}
}
}