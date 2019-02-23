#include <unordered_map>
#include <windows.h>
#include <stdexcept>
#include "global.h"
#include "utils.h"

namespace
{
	DWORD mode;

	void enableRawMode();
	void disableRawMode();

	std::unordered_map<WORD, int> keymap = {
		{ 33, PAGE_UP },
		{ 34, PAGE_DOWN },
		{ 35, END },
		{ 36, HOME },
		{ 37, ARROW_LEFT },
		{ 38, ARROW_UP },
		{ 39, ARROW_RIGHT },
		{ 40, ARROW_DOWN }
	};
}

int utils::getch()
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

		WORD keycode = input.Event.KeyEvent.wVirtualKeyCode;
		WORD unicode = input.Event.KeyEvent.uChar.UnicodeChar;

		auto key = keymap.find(keycode);
		retval = (key != keymap.end()) ? key->second : unicode;
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
}
