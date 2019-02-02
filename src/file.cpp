#include "file.h"

File::File(const std::string& path, IOS::openmode mode)
	: path(path)
{
	file.open(path.c_str(), mode);
}

File::~File() { file.close(); }

bool File::eof() const { return file.eof(); }

bool File::good() const { return file.good(); }

bool File::empty()
{
	return file.peek() == std::ifstream::traits_type::eof();
}

void File::close() { file.close(); }

bool File::is_open() const { return file.is_open(); }

std::string File::next_line()
{
	std::string str;
	std::getline(file, str);
	return str;
}

std::string File::get_path() const { return path; }

bool File::open(std::string path, IOS::openmode mode)
{
	this->path = path;
	return open(mode);
}

bool File::open(IOS::openmode mode)
{
	if (is_open()) file.close();
	file.open(path.c_str(), mode);
	return good();
}