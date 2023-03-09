#include "pch.h"
#include "StringUtils.h"

std::string Base::utils::ReplaceWith(std::string str, const std::string& replace,const std::string& token)
{
    const auto tokenLocation = str.find_first_of(token);
    if (tokenLocation != std::string::npos)
    {
        str.replace(tokenLocation, token.length(), replace);
    }
    return str;
}