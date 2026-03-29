#ifndef _0EXECTION0_
#define _0EXECTION0_

#include <pservice_base>
#include <future>
#include <vector>

#include <limits>
#include <vector>
#include "printer.hpp"
#include "exception.hpp"

#include "generator.hpp"
#include "constant.hpp"
#include "bitstring.hpp"
#include "function.hpp"

#include "local_outcome.hpp"

STD_PSERVICE_BEGIN

//------------------------------------------------
// component methods:

size_t decimal(const bitstring& b, const size_t i0, const size_t i1)
{
    size_t value = 0;
    for (size_t i = i0; i < i1; i++)
    {
        value *= 2;
        value += b[i];
    }

    return value;
}

double convert(const size_t decimal, const function& f)
{
    double value = f.get_minimum();
    value += decimal * (f.get_maximum() - f.get_minimum()) / 
        (pow(2, f.get_n()) - 1);
    return value;
}

double* convert(const bitstring& b, const function& f, 
    const size_t dimension)
{
    double* numbers = new double[dimension] {};
    for (size_t index_number = 0; index_number < dimension; index_number++)
    {
        size_t index_frst_bit = f.get_n() * index_number;
        size_t index_last_bit = f.get_n() * (index_number + 1);
        size_t value = decimal(b, index_frst_bit, index_last_bit);
        numbers[index_number] = convert(value, f);
    }

    return numbers;
}

// modifies just one number after one bit flip
bool improve(bitstring& candidate, const function& f,
    const strategy_type imprv, const size_t dimension)
{
    double* numbers = convert(candidate, f, dimension);
    double f_value_current = f.exe(numbers, dimension);
    double f_value_imprv = f_value_current;
    long long index_bit_imprv = -1;
    // the candidate bitstring is not changed in the loop
    // it is changed at the end with this cached index

    // for each number
    size_t index_frst = 0, index_last = f.get_n();
    for (size_t d = 0; d < dimension; d++)
    {
        // for each bit of the number
        for (size_t index_bit = index_frst;
            index_bit < index_last; index_bit++)
        {
            candidate[index_bit] = !candidate[index_bit];
            double previous = numbers[d];

            // evaluates f again with the new number as argument
            double value = convert(decimal(
                candidate, index_frst, index_last), f);
            numbers[d] = value;
            double f_value_neighborhood =
                f.exe(numbers, dimension);

            numbers[d] = previous;
            candidate[index_bit] = !candidate[index_bit];

            if (setting::is_better(f_value_neighborhood, f_value_current))
            {
                index_bit_imprv = index_bit;

                switch (imprv)
                {
                case strategy_type::best:
                    if (setting::is_better(f_value_neighborhood, f_value_imprv))
                        f_value_imprv = f_value_neighborhood;
                    break;
                case strategy_type::worst:
                    if (f_value_imprv == f_value_current ||
                        setting::is_better(f_value_imprv, f_value_neighborhood))
                        f_value_imprv = f_value_neighborhood;
                    break;
                default: // first
                    candidate[index_bit_imprv] = !candidate[index_bit_imprv];
                    return true;
                }
            }
        }

        index_frst += f.get_n();
        index_last += f.get_n();
    }

    // all hamming one neighbours were analysed
    delete[]numbers;
    if (f_value_imprv != f_value_current) // -1 != index_bit
    {
        candidate[index_bit_imprv] = !candidate[index_bit_imprv];
        return true;
    }

    return false;
}

void temperature_update(double& temperature, 
    double init, size_t it)
{
    double cooling_schedule = 0.985;
    temperature = init * std::pow(cooling_schedule, it);
}

//------------------------------------------------
// default method:

local_outcome hillclimbing(const function& f, 
    const strategy_type imprv, const size_t dimension)
{
    time_measurement clock;
    bitstring representation(dimension * f.get_n());
    while (improve(representation, f, imprv, dimension));

    double* numbers = convert(representation, f, dimension);
    double value_candidate = f.exe(numbers, dimension);
    delete[]numbers;

    long long time = clock.stop(time_unit::millisecond);
    if (0 == time)
        throw exception_null();
    
    return { value_candidate, time };
}

//------------------------------------------------
// thread manipulation:

void parallel_iteration(local_outcome& o,
    const size_t n_thread, const function& f, 
    const strategy_type& imprv, const size_t dimension)
{
    // async execution
    std::vector<std::future<local_outcome>> output(n_thread);
    for (size_t i_thread = 0; i_thread < n_thread; i_thread++)
        output[i_thread] = std::async(std::launch::async,
            hillclimbing, f, imprv, dimension);
    
    // query of the data
    for (size_t i_thread = 0; i_thread <= n_thread; i_thread++)
    {
        local_outcome candidate = output[i_thread].get();
        if (setting::is_better(candidate.optimum, o.optimum))
            o.optimum = candidate.optimum;
        o.time_measurement += candidate.time_measurement;
    }
}

//------------------------------------------------
// iterated methods:

/* returns the total time for all iterations */
local_outcome iterated_hillclimbing(const function& f, 
    const strategy_type imprv, 
    const size_t dimension, 
    const size_t it_nr)
{
    //if (it_nr < 100) // weak statistical significance
        //throw exception_null();

    const unsigned int n_thread = std::thread::hardware_concurrency();
    double value_init = std::numeric_limits<double>::infinity();
    if (setting::objective == objective_type::maximum_point)
        value_init *= -1;

    local_outcome outcome{ value_init, 0 };
    size_t it = 0;

    for (; it + n_thread < it_nr; it += n_thread)
        parallel_iteration(outcome, n_thread, f, imprv, dimension);
    parallel_iteration(outcome, it_nr - it, f, imprv, dimension);
    
    outcome.time_measurement;
    return outcome;
}

local_outcome simulated_annealing(const function& f, const size_t dimension)
{
    time_measurement clock;
    
    // temperature initialization
    double temperature_init = (double)dimension;
    switch (f.get_n()) // f_max 1D
    {
    case 0: // de jong 1
        temperature_init  *= 1;
        break;
    case 1: // rastrigin
        temperature_init  *= 40;
        break;
    case 2: // schwefel
        temperature_init  *= 840;
        break;
    case 3: // michalewicz
        temperature_init  *= 2;
        break;
    default:
        break;
    }
    temperature_init /= abs(log(0.8));
    double temperature = temperature_init;
    double temperature_threshold = temperature * 1e-8;
    
    // vc
    bitstring representation(dimension * f.get_n());
    double* numbers = convert(representation, f, dimension);
    double local_optimum = f.exe(numbers, dimension);
    delete[]numbers;

    // loop
    size_t it = 0, stagnation = 0;
    while(temperature > temperature_threshold && stagnation < SA_MAX_STAGNATION)
    {
        // select at random a neigbour
        double* numbers = convert(representation, f, dimension);
        double value_candidate = f.exe(numbers, dimension);
        delete[]numbers;

        // update
        if (setting::is_better(value_candidate, local_optimum))
            local_optimum = value_candidate;
        else
        {
            random_generator g;
            double probability = std::exp(-1 * abs(value_candidate - local_optimum) / temperature);
            if (g(0, 1) < probability)
                local_optimum = value_candidate;
        }

        temperature_update(temperature, temperature_init, it);
        if (++it == ITERATIONS_NUMBER)
            break;
    }

    // to change such that it can modify the current bitstring
    // iterated_hillclimbing(f, strategy_type::best, dimension, ITERATIONS_NUMBER - it);

    // stop clock
    long long milliseconds = clock.stop(time_unit::millisecond);
    if (0 == milliseconds)
        throw exception_null();

    return { local_optimum, milliseconds };
}

STD_PSERVICE_END
#endif
