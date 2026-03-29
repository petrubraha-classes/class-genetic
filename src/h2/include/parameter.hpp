#ifndef _0PARAMETER0_
#define _0PARAMETER0_

enum class objective_type
{
	minimum_point, maximum_point
};

enum class strategy_type
{
	best, first, worst, sa
};

struct parameter
{
	static objective_type objective;
	static int precision;
	static int dimension;
	static double mutation_probability;
	static double creaxion_probability; // cross-over rate

	static constexpr double poor_value()
	{
		return (objective_type::maximum_point == objective ?
			-1 : 1) * std::numeric_limits<double>::infinity();
	}

	static bool is_better(double candidate, double current)
	{
		if (objective_type::minimum_point == objective)
		{
			if (candidate < current)
				return true;
			return false;
		}

		if (candidate > current) // maximum_point
			return true;
		return false;
	}
};

#endif
