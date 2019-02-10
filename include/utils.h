#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

bool is_space(char ch);
std::string strip(std::string& str);
std::string strip(std::string&& str);
bool is_file_exist(const std::string& path);
size_t string_to_size_t(const std::string& str);
void unescape_character(std::string& str, const char ch);
std::vector<std::string> split(const std::string& s, char delimiter);
size_t find_first_nonescaped(const std::string& str, char ch);

#endif
