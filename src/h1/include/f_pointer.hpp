#ifndef _0FUNCTION_POINTER0_
#define _0FUNCTION_POINTER0_

#include <pservice_base>
#include <cmath>

STD_PSERVICE_BEGIN

typedef double (*fct_ptr)(const double* const numbers,
    const size_t dimension);

double fctptr_dejong1(const double* const x, const size_t n)
{
    double s = 0;
    for(size_t i = 0; i < n; i++)
        s += x[i] * x[i];
    
    return s;
}

double fctptr_michalewicz(const double* const x, const size_t n)
{
    const int m = 10;
    double s = 0;

    for(size_t i = 0; i < n; i++)
        s = s + sin(x[i]) * pow( (sin( i * x[i] * x[i] / PI )), (2 * m));
    
    return -s;
}

double fctptr_rastrigin(const double* const x, const size_t n)
{
    double s = 0.0;

    for(unsigned char i = 0; i < n; i++)
        s = s + (x[i]*x[i] - 10*cos(2*PI * x[i]));

    return 10 * n + s;
}

double fctptr_schwefel(const double* const x, const size_t n)
{
    double s = 0;
    for(unsigned char i = 0; i < n; i++)
        s += (-x[i] * sin(sqrt(abs(x[i]))));
    
    return s + 418.9829 * n;
}

STD_PSERVICE_END
#endif
