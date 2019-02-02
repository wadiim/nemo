#include "color.h"

std::ostream& color::red(std::ostream& stream)
{
	stream << "\033[31m";
	return stream;
}

std::ostream& color::green(std::ostream& stream)
{
	stream << "\033[32m";
	return stream;
}

std::ostream& color::reset(std::ostream& stream)
{
	stream << "\033[0m";
	return stream;
}
