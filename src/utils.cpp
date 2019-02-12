#include "utils.h"
#include <sstream>
#include <fstream>
#include <utility>
#include <cctype>

bool is_space(char ch)
{
	return std::isspace(static_cast<unsigned char>(std::move(ch)));
}

std::string strip(std::string& str)
{
	std::string s = str;
	return strip(std::move(s));
}

std::string strip(std::string&& str)
{
	while (str.size() && is_space(str.back())) str.pop_back();
	while (str.size() && is_space(str.front())) str.erase(str.begin());
	return str;
}

bool is_file_exist(const std::string& path)
{
	std::fstream file;
	bool retval{ false };

	file.open(path.c_str(), std::ios::in);
	if (file.good()) retval = true;
	file.close();
	return retval;
}

size_t string_to_size_t(const std::string& str)
{
	std::stringstream sstream(str);
	size_t retval;
	sstream >> retval;
	return retval;
}

void unescape_character(std::string& str, const char ch)
{
	size_t i = str.find(ch, 1);

	while (i != std::string::npos)
	{
		if (str[i - 1] == '\\') str.erase(i - 1, 1);
		i = str.find(ch, i + 1);
	}
}

std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::string token;
	std::vector<std::string> tokens;
	std::istringstream tokenStream(s);

	while (std::getline(tokenStream, token, delimiter))
		tokens.push_back(token);

	return tokens;
}

size_t find_first_nonescaped(const std::string& str, char ch)
{
	size_t i = str.find(ch);

	while (i != std::string::npos)
	{
		if (!i || (i && str[i - 1] != '\\')) return i;
		else i = str.find('-', i + 1);
	}

	return str.length();
}
