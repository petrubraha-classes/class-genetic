#ifndef _0CONSTANT0_
#define _0CONSTANT0_

#include "parameter.hpp"

typedef double (*fct_ptr)(const double* const numbers,
	const size_t dimension);

constexpr double PI = 3.14159265358979323846;
constexpr double EPS = 0.00001;

constexpr size_t SAMPLE_NUMBER = 30;
constexpr size_t ITERATIONS_NUMBER = 10000;
constexpr size_t SA_MAX_STAGNATION = 5;
constexpr size_t PRECISION = 5;

constexpr size_t POPULATION_SIZE = 100;
constexpr size_t GENERATIONS_NUMBER = 2024;
constexpr size_t GA_MAX_STAGNATION = GENERATIONS_NUMBER / 2;

objective_type parameter::objective = objective_type::minimum_point;
int parameter::precision = 5;
int parameter::dimension = 30;

// MUTATION_NUMBER mutations per chromosome
constexpr double MUTATION_NUMBER = 1.0;
double parameter::mutation_probability = MUTATION_NUMBER / 100;
double parameter::creaxion_probability = 0.50;

#endif
