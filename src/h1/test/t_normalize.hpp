#ifndef _0TEST30_
#define _0TEST30_

#include "printer.hpp"

void t_normalize()
{
	double number = 1.01010101515, nr = 1.01;
	std::cout << pservice::normalize(number) << ' ' << pservice::normalize(nr);

	exit(EXIT_SUCCESS);
}

#endif
