#include <stdexcept>
#include <iostream>
#include "nemo.h"
#include "term.h"

int main()
{
	try
	{
		term::term_init();
		Nemo nemo;
		nemo.menu();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
}
