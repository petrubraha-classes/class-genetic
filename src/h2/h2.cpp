#include <stdlib.h>
#include <filesystem>
#include <iostream>
#include <future>
#include <thread>

#include "constant.hpp"
#include "clock.hpp"
#include "printer.hpp"

#include "bitstring.hpp"
#include "function.hpp"
#include "execution 2.hpp"

#include "computation.hpp"

using namespace pservice;

static void parallel_sampling(const function& f, printer& file)
{
    std::vector<outcome> output;
    std::vector<std::thread> threads;
    for (size_t i_thread = 0; i_thread < SAMPLE_NUMBER; i_thread++)
    {
        //threads.emplace_back( /*lambda*/ [&]() {
            //output[i_thread] =
                //genetic_algorithm(f, GENERATIONS_NUMBER);
            //});
        threads.emplace_back(heavy_computation);
    }

    for (size_t i_thread = 0; i_thread < SAMPLE_NUMBER; i_thread++)
    {
        threads.at(i_thread).join();
        //file << output.at(i_thread);
        //std::cout << "samples analysed: " << i_thread + 1 << '\n';
    }
}

// for each dimension
template <bool parallel = false>
static void run(const function&);

template <>
static void run<true>(const function& f)
{
    size_t it = 0;

    parameter::dimension = 10;
    printer file_10(f);
    //ga_print_header(f);
    parallel_sampling(f, file_10);
    file_10.close();
    
    /*
    parameter::dimension = 30;
    printer file_30(f);
    //ga_print_header(f);
    parallel_sampling(f, file_10);
    file_10.close();
    */
}

template<>
static void run<false>(const function& f)
{
    parameter::dimension = 10;
    printer file_10(f);
    //ga_print_header(f);
    for (size_t it = 0; it < SAMPLE_NUMBER; it++)
    {
        outcome o = genetic_algorithm(f, GENERATIONS_NUMBER);
        file_10 << o;
        // std::cout << "samples analysed: " << it + 1 << '\n';
    }
    file_10.close();
    /*
    parameter::dimension = 30;
    printer file_30(f);
    //ga_print_header(f);
    for (size_t it = 0; it < SAMPLE_NUMBER; it++)
    {
        outcome o = genetic_algorithm(f, GENERATIONS_NUMBER);
        file_30 << o;
        std::cout << "samples analysed: " << it + 1 << '\n';
    }
    file_30.close();
    */
}

// to do: update fitness_function(), constant()

using it_dir = std::filesystem::recursive_directory_iterator;
void remove_previous()
{
    for (const auto& sub_path :
        it_dir("../output/fct_id 0/genetic algorithm/"))
        std::filesystem::remove_all(sub_path);
    for (const auto& sub_path :
        it_dir("../output/fct_id 1/genetic algorithm/"))
        std::filesystem::remove_all(sub_path);
    for (const auto& sub_path :
        it_dir("../output/fct_id 2/genetic algorithm/"))
        std::filesystem::remove_all(sub_path);
    for (const auto& sub_path :
        it_dir("../output/fct_id 3/genetic algorithm/"))
        std::filesystem::remove_all(sub_path);
}

void experiment(const function& f)
{
    std::cout << "non-concurrent times, concurrent times\n";
    for (size_t i = 0; i < SAMPLE_NUMBER; i++)
    {
        time_measurement clock_experiment;
        
        run<false>(f);
        std::cout << clock_experiment.stop(time_unit::second) << ' ';
        clock_experiment.start();

        run<true>(f);
        std::cout << clock_experiment.stop(time_unit::second) << '\n';
    }
}

int main()
{
    const function de_jong_1(-5.12, 5.12, fctptr_dejong1); 
    const function rastrigin(-5.12, 5.12, fctptr_rastrigin);
    const function schwefel(-500.0, 500.0, fctptr_schwefel);
    const function michalewicz(0, PI, fctptr_michalewicz);

    remove_previous();
    time_measurement clock;

    //run<>(de_jong_1);
    //run<>(rastrigin);
    //run<>(schwefel);
    //run<>(michalewicz);

    experiment(michalewicz);

    std::cout << "the program ran for " 
        << clock.stop(time_unit::minute) << " minutes.\n";
    return EXIT_SUCCESS;
}
