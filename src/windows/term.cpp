#include <windows.h>
#include <stdexcept>
#include <iostream>
#include "buffer.h"
#include "global.h"
#include "term.h"

HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
HANDLE hOut, original;
Buffer bOut;

void term::clear_screen()
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

void term::switch_to_normal_buffer()
{
	if (!SetConsoleActiveScreenBuffer(original))
		throw std::runtime_error("SetConsoleActiveScreenBuffer");
}

void term::switch_to_alternate_buffer()
{
	original = GetStdHandle(STD_OUTPUT_HANDLE);
	hOut = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL);

	if (original == INVALID_HANDLE_VALUE ||
		hOut == INVALID_HANDLE_VALUE)
		throw std::runtime_error("CreateConsoleScreenBuffer");
	if (!SetConsoleActiveScreenBuffer(hOut))
		throw std::runtime_error("SetConsoleActiveScreenBuffer");

	bOut.set_handle(hOut);
	std::cout.rdbuf(&bOut);

	atexit(switch_to_normal_buffer);
}
