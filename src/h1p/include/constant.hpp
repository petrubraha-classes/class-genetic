#ifndef _0CONSTANT0_
#define _0CONSTANT0_

#include <pservice_base>

STD_PSERVICE_BEGIN

#define ERROR_CODE -1

#define SAMPLE_NUMBER 30
#define ITERATIONS_NUMBER 10000

#define TO_PRECISION 100000
#define PRECISION 5
#define PI 3.14159

enum class objective_type // not now
{
    minimum_point, maximum_point
};

enum class improvement_type
{
    best, first, worst
};

enum class solution_type
{
    best, average, worst
};

void normalize(double& number)
{
    number = (double)((long long)(number * TO_PRECISION)) / TO_PRECISION;
}

STD_PSERVICE_END
#endif
