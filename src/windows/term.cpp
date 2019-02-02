#include <windows.h>
#include <stdexcept>

namespace
{
	DWORD mode;
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	void enableRawMode();
	void disableRawMode();
	bool isAlpha(const INPUT_RECORD& in);
	bool isUpper(const INPUT_RECORD& in);
}

int get_char()
{
	int retval = 0;
	DWORD nread;
	INPUT_RECORD input;

	enableRawMode();

	while (!retval)
	{
		do
		{
			ReadConsoleInputW(hIn, &input, 1, &nread);
		} while (input.EventType != KEY_EVENT ||
			!input.Event.KeyEvent.bKeyDown);

		if (isAlpha(input))
		{
			retval = input.Event.KeyEvent.wVirtualKeyCode +
				(isUpper(input) ? 0 : 32);
		}
		else retval = input.Event.KeyEvent.uChar.UnicodeChar;
	}

	disableRawMode();
	return retval;
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

namespace
{
	void enableRawMode()
	{
		if (hIn == NULL) throw std::runtime_error("Console not found");
		if (!GetConsoleMode(hIn, &mode))
			throw std::runtime_error("GetConsoleMode");
		if (!(SetConsoleMode(hIn, mode & ~(
			ENABLE_LINE_INPUT |
			ENABLE_ECHO_INPUT |
			ENABLE_PROCESSED_INPUT |
			ENABLE_PROCESSED_OUTPUT |
			ENABLE_WRAP_AT_EOL_OUTPUT
			)))) throw std::runtime_error("SetConsoleMode");
	}

	void disableRawMode()
	{
		if (!SetConsoleMode(hIn, mode))
			throw std::runtime_error("SetConsoleMode");
	}

	bool isAlpha(const INPUT_RECORD& in)
	{
		return in.Event.KeyEvent.wVirtualKeyCode >= 65 &&
			in.Event.KeyEvent.wVirtualKeyCode <= 90 ||
			in.Event.KeyEvent.wVirtualKeyCode >= 97 &&
			in.Event.KeyEvent.wVirtualKeyCode <= 122;
	}

	bool isUpper(const INPUT_RECORD& in)
	{
		return in.Event.KeyEvent.wVirtualKeyCode >= 65 &&
			in.Event.KeyEvent.wVirtualKeyCode <= 90 &&
			in.Event.KeyEvent.dwControlKeyState == CAPSLOCK_ON &&
			in.Event.KeyEvent.dwControlKeyState != SHIFT_PRESSED ||
			in.Event.KeyEvent.dwControlKeyState != CAPSLOCK_ON &&
			in.Event.KeyEvent.dwControlKeyState == SHIFT_PRESSED;
	}

}