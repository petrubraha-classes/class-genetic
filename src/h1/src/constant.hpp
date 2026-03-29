#ifndef _0CONSTANT0_
#define _0CONSTANT0_

#include <pservice_base>

STD_PSERVICE_BEGIN

constexpr double PI = 3.14159265358979323846;

constexpr size_t SAMPLE_NUMBER = 30;
constexpr size_t ITERATIONS_NUMBER = 10000;
constexpr size_t SA_MAX_STAGNATION = 5;
constexpr size_t PRECISION = 5;

enum class objective_type
{
    minimum_point, maximum_point
};

enum class strategy_type
{
    best, first, worst, sa
};

STD_PSERVICE_END
#endif
