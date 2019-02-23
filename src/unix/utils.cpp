#include <stdexcept>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include "utils.h"

namespace
{
	struct termios orig_termios;

	int get_byte(void* c);
	void enable_raw_mode();
	void disable_raw_mode();
	int parse_escape_sequence();
}

int utils::getch()
{
	enable_raw_mode();

	int c{};
	while (get_byte(&c) != 1);
	if (c == ESC) c = parse_escape_sequence();

	disable_raw_mode();
	return c;
}

namespace
{
	int get_byte(void* c)
	{
		ssize_t nread = read(STDIN_FILENO, c, 1);
		if (nread == -1 && errno != EAGAIN)
			throw std::runtime_error("read");
		return nread;
	}

	void enable_raw_mode()
	{
		struct termios raw = orig_termios;
		if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
			throw std::runtime_error("tcgetattr");
		raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
		raw.c_oflag &= ~(OPOST);
		raw.c_cflag |= (CS8);
		raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
		raw.c_cc[VMIN] = 0;
		raw.c_cc[VTIME] = 1;
		if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
			throw std::runtime_error("tcsetattr");
	}

	void disable_raw_mode()
	{
		if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
			throw std::runtime_error("tcsetattr");
	}

	int parse_escape_sequence()
	{
		unsigned char buf[4] = { 0 };

		if (get_byte(&buf[0]) == -1) return ESC;
		if (get_byte(&buf[1]) == -1) return ESC;

		switch (buf[0])
		{
		case '[':
			if (buf[1] >= '0' && buf[1] <= '9')
			{
				if (get_byte(&buf[2]) == -1) return ESC;

				if (buf[2] == '~')
				{
					switch (buf[1])
					{
					case '1': return HOME;
					case '4': return END;
					case '5': return PAGE_UP;
					case '6': return PAGE_DOWN;
					case '7': return HOME;
					case '8': return END;
					}
				}
			}
			else
			{
				switch (buf[1])
				{
				case 'A': return ARROW_UP;
				case 'B': return ARROW_DOWN;
				case 'C': return ARROW_RIGHT;
				case 'D': return ARROW_LEFT;
				case 'F': return END;
				case 'H': return HOME;
				}
			}
		case 'O':
			switch (buf[1])
			{
			case 'A': return ARROW_UP;
			case 'B': return ARROW_DOWN;
			case 'C': return ARROW_LEFT;
			case 'D': return ARROW_RIGHT;
			}
		}
		return ESC;
	}
}
