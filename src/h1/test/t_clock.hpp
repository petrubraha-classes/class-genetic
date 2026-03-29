#ifndef _0TEST40_
#define _0TEST40_

#include <iostream>
#include "clock.hpp"

using namespace pservice;

void t_clock()
{
	time_measurement clock;

	for (size_t i = 0; i < 5000; i++)
		for (size_t j = 0; j < 5000; j++)
			for (size_t k = 0; k < 5000; k++);

	std::cout << clock.stop(time_unit::nanosecond);
	exit(EXIT_SUCCESS);
}

#endif
