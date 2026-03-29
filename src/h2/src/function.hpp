#ifndef _0FUnCTIOn0_
#define _0FUnCTIOn0_

#include <pservice_base>
#include "f_pointer.hpp"
#include "constant.hpp"

STD_PSERVICE_BEGIN

class function
{
    // data members:
    static unsigned int unused_id;

    unsigned int n, id;
    const double minimum;
    const double maximum;
    fct_ptr f;

public:
    // constructors:
    ~function() = default;
    function() = delete;
    function(double, double, fct_ptr);
    
    // specific methods:
    double exe(const double* const, const size_t) const;
    double exe(const double* const) const;

    // constant methods:
    const unsigned int& get_n() const;
    const unsigned int& get_id() const;
    const double& get_minimum() const;
    const double& get_maximum() const;
    fct_ptr get_pointer() const;
};

//------------------------------------------------
// constructors:

unsigned int function::unused_id = 0;

function::function(double min, double max, fct_ptr ptr) :
    n(0), id(unused_id++), minimum(min), maximum(max), f(ptr)
{
    double pw = std::pow(10, PRECISION);

    n = (unsigned int)((maximum - minimum) * pw);
    n = (unsigned int)ceil(log2(n));
}

//------------------------------------------------
// specific methods:

// for hill-climbing and simulated annealing
double function::exe(const double* const numbers, 
    const size_t dimension) const
{
    return f(numbers, dimension);
}

// for genetic algorithm
double function::exe(const double* const numbers) const
{
    return f(numbers, parameter::dimension);
}

//------------------------------------------------
// constant methods:

const unsigned int& function::get_n() const
{
    return n;
}

const unsigned int& function::get_id() const
{
    return id;
}

const double& function::get_minimum() const
{
    return minimum;
}

const double& function::get_maximum() const
{
    return maximum;
}

fct_ptr function::get_pointer() const
{
    return f;
}

STD_PSERVICE_END
#endif
