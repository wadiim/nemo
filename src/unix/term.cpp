#include <stdexcept>
#include <unistd.h>

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
