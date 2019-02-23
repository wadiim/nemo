#ifndef TERM_H
#define TERM_H

#include <utility>

namespace term
{
	void init();
	void clear_screen();
	void switch_to_normal_buffer();
	void switch_to_alternate_buffer();
	std::pair<size_t, size_t> get_window_size();
}

#endif
