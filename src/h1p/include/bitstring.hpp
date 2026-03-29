#ifndef _0BITSTRING0_
#define _0BITSTRING0_

#include <pservice_base>
#include <vector>

#include "generator.hpp"
#include "constant.hpp"

/* comments:
* it don't have to consider here the minimum, maximum and precision
* it's a task for the developer from future
*/

STD_PSERVICE_BEGIN

class bitstring
{
    std::vector<bool> bits;

public:
    // constructors:
    ~bitstring() = default;
    bitstring() = delete;
    bitstring(size_t);
    bitstring(int, bool);
    bitstring(const std::initializer_list<bool>&);

    // constant methods:
    auto operator [](const size_t index) const;
    auto operator [] (const size_t);
    size_t size() const;
    void print(std::ostream&) const;
};

//------------------------------------------------
// constructors:

bitstring::bitstring(size_t n) : bits(n)
{
    random_generator g;
    for (size_t i = 0; i < n; i++)
        bits[i] = g() % 2;
}

bitstring::bitstring(int number, bool dummy) :
    bits(5)
{
    if (number > 31)
        throw - 1;

    for (size_t i = 0; i < 5; i++)
        bits[i] = 0;

    size_t index = 4;
    while (number)
    {
        bool remainder = number % 2;
        number /= 2;
        bits[index] = remainder;
        index--;
    }
}

bitstring::bitstring(const std::initializer_list<bool>& list) :
    bits(list) {}

//------------------------------------------------
// constant methods:

auto bitstring::operator [] (const size_t index) const
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

void bitstring::print(std::ostream& out = std::cout) const
{
    for (size_t i = 0; i < bits.size(); i++)
        out << bits[i];
    out << ' ';
}

STD_PSERVICE_END
#endif
