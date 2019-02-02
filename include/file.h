#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <fstream>
#include <string>

class File
{
	typedef std::ios_base IOS;

public:
	File(const std::string& path, IOS::openmode mode = IOS::in | IOS::out);
	File() = default;
	~File();

	bool eof() const;
	bool good() const;
	bool empty();
	void close();
	bool is_open() const;
	std::string next_line();
	std::string get_path() const;
	bool open(IOS::openmode mode = IOS::in | IOS::out);
	bool open(std::string path, IOS::openmode mode = IOS::in | IOS::out);

	template<typename T> friend File& operator<<(File& f, T data);

private:
	std::fstream file;
	std::string path;
};

#include "file.ipp"

#endif
