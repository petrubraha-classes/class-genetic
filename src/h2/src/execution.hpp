#ifndef _0EXECTION0_
#define _0EXECTION0_

#include <pservice_base>
#include <future>

#include <vector>
#include "clock.hpp"
#include "outcome.hpp"
#include "constant.hpp"
#include "exception.hpp"
#include "printer.hpp"

#include "generator.hpp"
#include "bitstring.hpp"
#include "function.hpp"

/* should allow repeating values from random generation?
* in the following code it is not allowed
*/

STD_PSERVICE_BEGIN

typedef bitstring chromosome;
typedef bool gene;

random_generator g;

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

//------------------------------------------------
// operators:

double constant(unsigned int obj_fct_id)
{
    if (3 == obj_fct_id) // michalewicz
        return 30;
    return 0;
}

double fitness_function(double candidate, unsigned int obj_fct_id)
{
    if (parameter::objective == objective_type::maximum_point)
    {
        // if it returns only positive values
        if (obj_fct_id != 3) // no michalewicz
            return candidate;
        
        // it returns negative values
        return candidate + constant(obj_fct_id);
    }

    // mimimum - positive function
    if (obj_fct_id != 3) // no michalewicz
        return 1 / candidate;
    
    // minimum - negative function
    return -1 * candidate;
}

// returns best value and computes the fitness
double evaluate(const std::vector<chromosome>& population, 
    std::vector<double>& population_fitness,
    const function& f)
{
    double fittest = parameter::poor_value();

    for (size_t i = 0; i < population.size(); i++)
    {
        // establish the best value
        double* numbers = convert(population.at(i), f, parameter::dimension);
        double candidate = f.exe(numbers);
        delete[]numbers;
        
        // compute the fitness for each function
        if (parameter::is_better(candidate, fittest))
            fittest = candidate;

        population_fitness[i] = fitness_function(candidate, f.get_id());
    }
    
    return fittest;
}

//------------------------------------------------

void select(std::vector<chromosome>& population,
    const std::vector<double>& population_fitness)
{
    // greater chance for those with a better fitness value
    double total_fitness = 0;
    for (size_t i = 0; i < population_fitness.size(); i++)
        total_fitness += population_fitness.at(i);
    
    std::vector<double> probabilities(population.size());
    for (size_t i = 0; i < population_fitness.size(); i++)
        probabilities[i] = 
        population_fitness.at(i) / total_fitness;

    std::vector<double> roulette(probabilities);
    for (size_t i = 1; i < roulette.size(); i++)
        roulette[i] += roulette.at(i - 1);

    // find next generation
    std::vector<size_t> index_presence(population.size());
    for (size_t i = 0; i < population.size(); i++)
    {
        double probability = g(0.0, 1.0);
        size_t spot = 0;
        while (spot < roulette.size() &&
            probability > roulette.at(spot))
            spot++;
        index_presence[spot]++;
    }

    std::vector<size_t> index_list_null;
    std::vector<size_t> index_list_real;
    for (size_t i = 0; i < index_presence.size(); i++)
    {
        if (0 == index_presence[i])
            index_list_null.emplace_back(i);
        else
            while (index_presence.at(i))
            {
                index_list_real.emplace_back(i);
                index_presence[i]--;
            }
    }
    
    // mistake: not random choice of the selected chromosomes
    size_t index_index_p = 0;
    for (size_t i = 0; i < index_list_null.size(); i++)
    {
        population[index_list_null.at(i)] = 
            population[index_list_real.at(index_index_p)];
        index_index_p++;
    }
}

//------------------------------------------------

// first implementation - one cutting point
void cross_over(std::vector<chromosome>& population)
{
    std::vector<size_t> indexes; 
    //(parameter::creaxion_probability * population.size());

    for (size_t index_chromosome = 0;
        index_chromosome < population.size(); index_chromosome++)
        if (g(0.0, 1.0) < parameter::creaxion_probability)
            indexes.emplace_back(index_chromosome);
    
    // odd number of chromosomes fix 
    // random decision to add / remove
    if (indexes.size() % 2 && g((size_t)0, (size_t)1))
    {
        size_t index_chromosome = 0;
        
        bool condition = true; // don't repeat any index
        do
        {
            condition = true;
            index_chromosome = g(0, population.size() - 1);
            
            for(size_t i = 0; i < indexes.size(); i++)
                if (indexes.at(i) == index_chromosome)
                {
                    condition = false;
                    break;
                }
        } while (false == condition);
        
        indexes.emplace_back(index_chromosome);
    }

    size_t gene_number = population.at(0).size();
    size_t nr_c = indexes.size();
    if (nr_c % 2) // there is one chromosome left out
        nr_c--; // it was refused to accept another one
    // in this case: at the end of the loop |indexes| = 1

    for (size_t it_c = 0; it_c < nr_c; it_c += 2)
    {
        // random mate
        size_t index_frst = 0, index_last = 0, ddd = indexes.size();
        index_frst = g(0, indexes.size() - 1);
        index_last = g(0, indexes.size() - 1);
        while(index_frst == index_last)
            index_last = g(0, indexes.size() - 1);
        
        // for more redable code
        size_t index_c0 = indexes[index_frst];
        size_t index_c1 = indexes[index_last];
        chromosome& c0 = population.at(index_c0);
        chromosome& c1 = population.at(index_c1);

        // erase
        indexes.erase(indexes.begin() + index_frst);
        if (index_frst < index_last)
            index_last--;
        indexes.erase(indexes.begin() + index_last);
        
        // actual change
        size_t index_gene = g(0, gene_number - 2);
        for (size_t i = index_gene + 1; i < gene_number; i++)
        {
            gene temp_gene = c0[i];
            c0[i] = c1[i];
            c1[i] = temp_gene;
        }
    }
}

std::vector<size_t>& index_push(
    std::vector<size_t>& arr, size_t to_push, size_t index_array)
{
    if (index_array >= arr.size())
    {
        arr.emplace_back(to_push);
        return arr;
    }

    arr[index_array] = to_push;
    return arr;
}

void breeding(chromosome& c0, chromosome& c1, 
    size_t n_cutting_point)
{
    size_t gene_number = c0.size();

    std::vector<size_t> indexes(n_cutting_point);
    for (size_t i = 0; i < n_cutting_point; i++)
    {
        size_t index_gene = 0;
        bool condition = false;
        while (false == condition)
        {
            condition = true;
            index_gene = g(0, gene_number - 2);
            for (size_t j = 0; j < i; j++)
                if (indexes.at(j) == index_gene)
                    condition = false;
        }

        indexes[i] = index_gene;
    }
    std::sort(indexes.begin(), indexes.end());

    size_t it_indexes = 0;
    while (it_indexes < indexes.size())
    {
        size_t index_frst = indexes.at(it_indexes);
        size_t index_last = gene_number - 1;
        it_indexes++;
        if (it_indexes < indexes.size())
            index_last = indexes.at(it_indexes);
        
        for (size_t i = index_frst; i < index_last; i++)
        {
            gene temp_gene = c0[i];
            c0[i] = c1[i];
            c1[i] = temp_gene;
        }

        it_indexes++;
    }
}

// second implementation - n cutting points
void cross_over(std::vector<chromosome>& population,
    size_t n_cutting_point)
{
    if (0 == n_cutting_point)
        n_cutting_point++;

    size_t n_i = population.size() *  
        (size_t)parameter::creaxion_probability;
    size_t i_i = 0, i_ni = 0;
    std::vector<size_t> indexes(n_i),
        not_indexes(population.size() - n_i);

    for (size_t index_chromosome = 0;
        index_chromosome < population.size(); index_chromosome++)
    {
        if (g(0.0, 1.0) < parameter::creaxion_probability)
        {
            index_push(indexes, index_chromosome, i_i);
            i_i++;
        }
        else
        {
            index_push(not_indexes, index_chromosome, i_ni);
            i_ni++;
        }
    }

    // odd number of chromosomes fix 
    // random decision to add / remove
    if (indexes.size() % 2 && g((size_t)0, (size_t)1))
    {
        size_t index_chromosome = g(0, not_indexes.size() - 1);
        indexes.emplace_back(not_indexes.at(index_chromosome));
    }

    size_t gene_number = population.at(0).size();
    size_t nr_c = indexes.size();
    if (nr_c % 2) // there is one chromosome left out
        nr_c--; // it was refused to accept another one
    // in this case: at the end of the loop |indexes| = 1

    for (size_t it_c = 0; it_c < nr_c; it_c += 2)
    {
        // random mate
        size_t index_frst = 0, index_last = 0, ddd = indexes.size();
        index_frst = g(0, indexes.size() - 1);
        index_last = g(0, indexes.size() - 1);
        while (index_frst == index_last)
            index_last = g(0, indexes.size() - 1);

        // for more redable code
        size_t index_c0 = indexes[index_frst];
        size_t index_c1 = indexes[index_last];
        chromosome& c0 = population.at(index_c0);
        chromosome& c1 = population.at(index_c1);

        // erase
        indexes.erase(indexes.begin() + index_frst);
        if (index_frst < index_last)
            index_last--;
        indexes.erase(indexes.begin() + index_last);

        breeding(c0, c1, n_cutting_point);
    }
}

void mutate(std::vector<chromosome>& population)
{
    size_t gene_number = population.at(0).size();
    for (chromosome& c : population)
        for (size_t index_gene = 0; index_gene < gene_number; index_gene++)
            if (g(0.0, 1.0) < parameter::mutation_probability)
                c[index_gene] = !c[index_gene];
}

//------------------------------------------------
// genetic algorithm:

outcome genetic_algorithm(const function& f, const size_t generations)
{
    time_measurement clock;

    std::vector<chromosome> population;
    for (size_t i = 0; i < POPULATION_SIZE; i++)
        population.emplace_back(f.get_n() * parameter::dimension);

    std::vector<double> population_fitness(population.size());
    double local_optimum = evaluate(population, population_fitness, f);

    size_t gene_number = population.at(0).size();
    parameter::mutation_probability = MUTATION_NUMBER / gene_number;

    size_t stagnation = 0;
    for (size_t index_g = 0; index_g < generations; index_g++)
    {
        double previous_optimum = local_optimum;
        
        // operators
        select(population, population_fitness);
        cross_over(population);
        mutate(population);
        local_optimum = evaluate(population, population_fitness, f);

        // another halting condition
        if (parameter::is_better(previous_optimum, local_optimum))
        {
            local_optimum = previous_optimum;
            stagnation++;
            if (GA_MAX_STAGNATION == stagnation)
                break;
        }
        else stagnation = 0;
    }

    return { local_optimum, clock.stop(time_unit::millisecond) };
}

void test_genetic_algorithm(const function& f)
{
    std::vector<chromosome> population;
    for (size_t i = 0; i < POPULATION_SIZE; i++)
        population.emplace_back(f.get_n() * parameter::dimension);

    std::vector<double> population_fitness(population.size());
    double local_optimum = evaluate(population, population_fitness, f);

    size_t gene_number = population.at(0).size();
    parameter::mutation_probability = MUTATION_NUMBER / gene_number;

    size_t stagnation = 0;
    for (size_t index_g = 0;; index_g++)
    {
        std::cout << local_optimum << ' ' << index_g << '\n';

        // operators
        select(population, population_fitness);
        cross_over(population);
        mutate(population);
        double current_best = evaluate(population, population_fitness, f);

        // another halting condition
        if (parameter::is_better(local_optimum, current_best))
        {
            stagnation++;
            if (GA_MAX_STAGNATION == stagnation)
                break;
        }
        else
        {
            stagnation = 0;
            local_optimum = current_best;
        }
    }

    std::cout << "global minimum: " << local_optimum << '\n';
}

STD_PSERVICE_END
#endif
