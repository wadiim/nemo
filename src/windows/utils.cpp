#include <windows.h>
#include <stdexcept>

namespace
{
	DWORD mode;
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

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
