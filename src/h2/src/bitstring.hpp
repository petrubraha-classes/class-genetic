#ifndef _0BITSTRING0_
#define _0BITSTRING0_

#include <pservice_base>
#include <vector>

#include "generator.hpp"
#include "constant.hpp"

/* comments:
* it doesn't consider here the minimum, maximum and precision
*/

STD_PSERVICE_BEGIN

class bitstring
{
    std::vector<bool> bits;

public:
    // constructors:
    ~bitstring() = default;
    bitstring() = delete;
    bitstring(const size_t);

    // constant methods:
    bool operator [] (const size_t) const;
    auto operator [] (const size_t);
    size_t size() const;
};

//------------------------------------------------
// constructors:

bitstring::bitstring(const size_t n) : bits(n)
{
    random_generator g;
    for (size_t i = 0; i < n; i++)
        bits[i] = g() % 2;
}

//------------------------------------------------
// constant methods:

bool bitstring::operator [] (const size_t index) const
{
    return bits[index];
}

auto bitstring::operator [] (const size_t index)
{
    return bits[index];
}

size_t bitstring::size() const
{
    return bits.size();
}

STD_PSERVICE_END
#endif
