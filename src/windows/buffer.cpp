#include "buffer.h"

void Buffer::set_handle(HANDLE h) { this->h = h; }

Buffer::int_type Buffer::overflow(int_type c)
{
	if (c != EOF)
	{
		DWORD written;
		WriteConsole(h, &c, 1, &written, nullptr);
	}
	return c;
}

std::streamsize Buffer::xsputn(const char_type* s, std::streamsize count)
{
	DWORD written;
	WriteConsole(h, s, static_cast<DWORD>(count), &written, nullptr);
	return static_cast<std::streamsize>(written);
}
