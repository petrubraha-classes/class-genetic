#ifndef _0TEST10_
#define _0TEST10_

#include <iostream>
#include <string>

#include "printer.hpp"

#include "f_pointer.hpp"
#include "function.hpp"

#include "local_outcome.hpp"

using namespace pservice;

void t_printer()
{
	const function de_jong_1(-5.12, 5.12, fctptr_dejong1);
	printer p(de_jong_1, strategy_type::best, 2);
	
	const std::string output =
		"../../output/fct_id 0/improve best/fct_id 0 improve best 2D.txt";
	std::cout << p.get_path() << '\n' << output;
	if (p.get_path() != output)
		exit(EXIT_FAILURE);

	exit(EXIT_SUCCESS);
}

void t_printer_print()
{
	const function de_jong_1(-5.12, 5.12, fctptr_dejong1);
	printer p(de_jong_1, strategy_type::best, 2);
	std::string message = "test";
	p << message;
	exit(EXIT_SUCCESS);
}

void t_printer_print_result()
{
	const function de_jong_1(-5.12, 5.12, fctptr_dejong1);
	printer p(de_jong_1, strategy_type::best, 2);

	local_outcome o{ 1, 331 };
	p << o;
	exit(EXIT_SUCCESS);
}

#endif
