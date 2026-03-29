#ifndef _0SETTING0_
#define _0SETTING0_

#include <pservice_base>
#include "constant.hpp"

STD_PSERVICE_BEGIN

struct setting
{
	static objective_type objective;
	static int precision;

	static bool is_better(double candidate, double current)
	{
		if (objective_type::minimum_point == objective)
		{
			if (candidate < current)
				return true;
			return false;
		}

		if (candidate > current)
			return true;
		return false;
	}
};

objective_type setting::objective = objective_type::minimum_point;
int setting::precision = 0;

STD_PSERVICE_END
#endif
