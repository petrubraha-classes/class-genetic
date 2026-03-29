#include <stdlib.h>
#include <iostream>
#include <thread>

#include "constant.hpp"
#include "setting.hpp"
#include "clock.hpp"
#include "printer.hpp"

#include "bitstring.hpp"
#include "function.hpp"
#include "f_pointer.hpp"
#include "execution.hpp"

#include "t_infinity.hpp"
#include "t_printer.hpp"
#include "t_normalize.hpp"
#include "t_clock.hpp"

using namespace pservice;

static void sa_analysis(const function&, size_t);
static void hc_analysis(const function&, size_t);
static void run(const function&);

//------------------------------------------------
// start:

int main()
{
    std::cout << "h1 program starts running.\n\n";
    srand((unsigned int)time(0));
    setting::objective = objective_type::minimum_point;
    setting::precision = PRECISION;
    
    const function de_jong_1(-5.12, 5.12, fctptr_dejong1); 
    const function rastrigin(-5.12, 5.12, fctptr_rastrigin);
    const function schwefel(-500.0, 500.0, fctptr_schwefel);
    const function michalewicz(0, PI, fctptr_michalewicz);

    time_measurement clock;

    run(de_jong_1);
    //run(rastrigin);
    //run(schwefel);
    //run(michalewicz);

    std::cout << "the program ran for " 
        << clock.stop(time_unit::minute) << " minutes.\n";
    return EXIT_SUCCESS;
}

static void hc_analysis(const function& f, const size_t dimension)
{
    // iterate through each imrovement
    size_t index_strat = 0;
    strategy_type strat = strategy_type::best;
    while (strategy_type::sa != strat)
    {
        print_analysis_header(f, strat, dimension, std::cout);

        local_outcome sample_outcome[SAMPLE_NUMBER]{};
        for (size_t index_sample = 0; index_sample < SAMPLE_NUMBER; index_sample++)
        {
            std::cout << "sample " << index_sample << '\n';
            sample_outcome[index_sample] = iterated_hillclimbing(f, strat, dimension, 2);
        }
        std::cout << '\n';

        printer file(f, strat, dimension);
        for (size_t index_sample = 0; index_sample < SAMPLE_NUMBER; index_sample++)
            file << sample_outcome[index_sample];

        strat = static_cast<strategy_type>(++index_strat);
    }
}

static void sa_analysis(const function& f, const size_t dimension)
{
    print_analysis_header(f, strategy_type::sa, dimension, std::cout);

    local_outcome sample_outcome[SAMPLE_NUMBER]{};
    for (size_t index_sample = 0; index_sample < SAMPLE_NUMBER; index_sample++)
    {
        std::cout << "sample " << index_sample << '\n';
        sample_outcome[index_sample] = simulated_annealing(f, dimension);
    }
    std::cout << '\n';

    printer file(f, strategy_type::sa, dimension);
    for (size_t index_sample = 0; index_sample < SAMPLE_NUMBER; index_sample++)
        file << sample_outcome[index_sample];
}

static void run(const function& f)
{
    size_t d = 5;
    //hc_analysis(f, d);
    sa_analysis(f, d);

    /*size_t d = 10;
    hc_analysis(f, d);
    sa_analysis(f, d);

    size_t d = 30;
    hc_analysis(f, d);
    sa_analysis(f, d);*/
}
