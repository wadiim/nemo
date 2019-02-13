#include <stdexcept>
#include <cstdlib>
#include "term.h"

void term::term_init()
{
	switch_to_alternate_buffer();
	if (std::atexit(switch_to_normal_buffer))
		throw std::runtime_error("Registration failed");
}
