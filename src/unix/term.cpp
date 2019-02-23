#include <sys/ioctl.h>
#include <stdexcept>
#include <unistd.h>
#include <tuple>
#include "term.h"

namespace
{
	std::pair<size_t, size_t> get_cursor_position();
}

void term::clear_screen()
{
	if (write(STDOUT_FILENO, "\x1b[2J", 4) != 4)
		throw std::runtime_error("write");
	if (write(STDOUT_FILENO, "\x1b[H", 3) != 3)
		throw std::runtime_error("write");
}

void term::switch_to_normal_buffer()
{
	if (write(STDOUT_FILENO, "\x1b[?1049l", 8) != 8)
		throw std::runtime_error("write");
}

void term::switch_to_alternate_buffer()
{
	if (write(STDOUT_FILENO, "\x1b[?1049h\x1b[H", 11) != 11)
		throw std::runtime_error("write");
}

std::pair<size_t, size_t> term::get_window_size()
{
	size_t x, y;
	struct winsize ws;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
	{
		if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
			throw std::runtime_error("write");
		std::tie(x, y) = get_cursor_position();
		++x; ++y;
	}
	else
	{
		x = ws.ws_col;
		y = ws.ws_row;
	}
	return std::make_pair(x, y);
}

namespace
{
	std::pair<size_t, size_t> get_cursor_position()
	{
		size_t x, y;
		char buf[32];
		unsigned i{};
		if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
			throw std::runtime_error("write");
		while (i < sizeof(buf) - 1)
		{
			if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
			if (buf[i] == 'R') break;
			++i;
		}
		buf[i] = '\0';
		if (buf[0] != '\x1b' || buf[1] != '[')
			throw std::runtime_error("get_cursor_position");
		if (sscanf(&buf[4], "%zu", &x) != 1)
			throw std::runtime_error("sscanf");
		if (sscanf(&buf[2], "%zu", &y) != 1)
			throw std::runtime_error("sscanf");
		return std::make_pair(--x, --y);
	}
}
