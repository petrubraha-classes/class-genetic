#ifndef _0CLOCK0_
#define _0CLOCK0_

#include <pservice_base>
#include "constant.hpp"
#include <chrono>
using namespace std::chrono;

STD_PSERVICE_BEGIN
#define TIME_UNIT_STRIDE 1000

enum class time_unit
{
    minute, second, millisecond, microsecond, nanosecond
    //      10 ^ 0  10 ^ (-3)    10 ^ (-6)    10 ^ (-9)
};

//------------------------------------------------
// class definition:

class time_measurement
{
    // data members:
    bool started;
    steady_clock::time_point start_point;

public:
    // constructors:
    ~time_measurement() = default;
    time_measurement(const time_measurement&) = delete;
    time_measurement(const time_measurement&&) = delete;
    time_measurement();

    // specific methods:
    bool start(); 
    long long stop(time_unit);

    // query operations:
    bool is_started() const;
    
    // constant methods:
    steady_clock::time_point get_start_point() const;
    double get_seconds() const;
};

//------------------------------------------------
// constructors:

time_measurement::time_measurement() :
    started(true), start_point(high_resolution_clock::now()) {}

//------------------------------------------------
// specific methods:

// automatically starts when constructed
// returns true if this call started the time
bool time_measurement::start()
{
    if (false == started)
    {
        start_point = high_resolution_clock::now();
        started = true;
        return true;
    }

    return false;
}

long long time_measurement::stop(time_unit u)
{
    if (false == started)
        return 0;
    
    started = false;
    auto elapsed_time = high_resolution_clock::now() - start_point;
    long long nns = elapsed_time.count();
    
    if (time_unit::nanosecond == u)
        return elapsed_time.count();

    if (time_unit::microsecond == u)
        return duration_cast<microseconds>(elapsed_time).count();

    if (time_unit::millisecond == u)
        return duration_cast<milliseconds>(elapsed_time).count();

    if(time_unit::second == u)
        return duration_cast<seconds>(elapsed_time).count();

    return duration_cast<minutes>(elapsed_time).count();
}

//------------------------------------------------
// query operations:

bool time_measurement::is_started() const
{
    return started;
}

//------------------------------------------------
// constant  methods:

steady_clock::time_point time_measurement::get_start_point() const
{
    return start_point;
}

double time_measurement::get_seconds() const
{
    if (false == started)
        return 0.0;

    steady_clock::time_point stop_point = high_resolution_clock::now();

    duration<double> elapsed_time = stop_point - start_point;
    return elapsed_time.count();
}

STD_PSERVICE_END
#endif
