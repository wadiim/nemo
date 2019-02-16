#ifndef BUFFER_H
#define BUFFER_H

#ifdef _WIN32
#include <streambuf>
#include <windows.h>

class Buffer : public std::streambuf
{
public:
	Buffer() {}
	Buffer(HANDLE h) : h(h) {}
	Buffer(const Buffer&) = delete;
	Buffer& operator=(const Buffer&) = delete;

	void set_handle(HANDLE h);

protected:
	virtual int_type overflow(int_type c) override;
	virtual std::streamsize xsputn(const char_type* s,
		std::streamsize count) override;

private:
	HANDLE h;
};

#endif
#endif
