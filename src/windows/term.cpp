#include <windows.h>
#include <stdexcept>

namespace
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
}

void clear_screen()
{
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten, dwConSize;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(hOut, &csbi))
		throw std::runtime_error("GetConsoleScreenBufferInfo");
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	if (!FillConsoleOutputCharacter(hOut, (TCHAR)' ',
		dwConSize, coordScreen, &cCharsWritten))
		throw std::runtime_error("FillConsoleOutputCharacter");
	if (!FillConsoleOutputAttribute(hOut, csbi.wAttributes,
		dwConSize, coordScreen, &cCharsWritten))
		throw std::runtime_error("FillConsoleOutputAttribute");
	if (!SetConsoleCursorPosition(hOut, coordScreen))
		throw std::runtime_error("SetConsoleCursorPosition");
}

void switch_to_normal_buffer() { clear_screen(); }

void switch_to_alternate_buffer() { clear_screen(); }
