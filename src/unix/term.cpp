#include <stdexcept>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

namespace
{
	struct termios orig_termios;

	void enable_raw_mode();
	void disable_raw_mode();
}

int get_char()
{
	int c, nread = 0;
	enable_raw_mode();

	while (nread != 1)
	{
		c = 0;
		nread = read(STDIN_FILENO, &c, 1);
		if (nread == -1 && errno != EAGAIN)
			throw std::runtime_error("read");
	}

	disable_raw_mode();
	return c;
}

void clear_screen()
{
	if (write(STDOUT_FILENO, "\x1b[2J", 4) != 4)
		throw std::runtime_error("write");
	if (write(STDOUT_FILENO, "\x1b[H", 3) != 3)
		throw std::runtime_error("write");
}

void switch_to_normal_buffer()
{
	if (write(STDOUT_FILENO, "\x1b[?1049l", 8) != 8)
		throw std::runtime_error("write");
}

void switch_to_alternate_buffer()
{
	if (write(STDOUT_FILENO, "\x1b[?1049h\x1b[H", 11) != 11)
		throw std::runtime_error("write");
}

namespace
{
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
}
