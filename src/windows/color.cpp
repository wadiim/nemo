#include <windows.h>
#include <stdexcept>
#include "color.h"

namespace
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD defaultAttributes = 0;

	void set_color(int color);
	void save_default_attributes();
}

std::ostream& color::red(std::ostream& stream)
{
	set_color(FOREGROUND_RED);
	return stream;
}

std::ostream& color::green(std::ostream& stream)
{
	set_color(FOREGROUND_GREEN);
	return stream;
}

std::ostream& color::reset(std::ostream& stream)
{
	SetConsoleTextAttribute(hOut, defaultAttributes);
	return stream;
}

namespace
{
	void set_color(int color)
	{
		save_default_attributes();
		SetConsoleTextAttribute(hOut, color);
	}

	void save_default_attributes()
	{
		if (defaultAttributes) return;

		CONSOLE_SCREEN_BUFFER_INFO info;
		if (!GetConsoleScreenBufferInfo(hOut, &info))
			throw std::runtime_error("GetConsoleScreenBufferInfo");
		defaultAttributes = info.wAttributes;
	}
}
