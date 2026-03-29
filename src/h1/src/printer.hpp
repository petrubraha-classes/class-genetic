#ifndef _0PRINTER0_
#define _0PRINTER0_

#include <pservice_base>
#include <iostream>
#include <fstream>
#include <string>

#include "exception.hpp"

#include "constant.hpp"
#include "function.hpp"
#include "local_outcome.hpp"

STD_PSERVICE_BEGIN

//------------------------------------------------
// methods:

void print_analysis_header(const function& f, strategy_type imprv,
    size_t dimension, std::ostream& out = std::cout)
{
    out << "fct_id " << f.get_id() << ", improve ";
    switch (imprv)
    {
    case strategy_type::best:
        out << "best";
        break;
    case strategy_type::first:
        out << "first";
        break;
    case strategy_type::worst:
        out << "worst";
        break;
    default:
        out << "simulated annealing";
        break;
    }

    out << ", " << dimension << "D.\n\n";
}

[[deprecated("creates chaos of the parallel running")]]
void print_iteration(size_t index, std::ostream& out = std::cout)
{
    if (0 == index % 100)
        out << index << "iterations.\n";
}

std::string normalize(double number)
{
    std::string s = std::to_string(number);
    size_t index = s.find('.');
    if (index != std::string::npos)
        return s.substr(0, index + 6);
    return s;
}

//------------------------------------------------
// classes:

const char path_header[] = "../../../";
const char path_folder[] = "output/";
const char path_footer[] = ".csv";

class printer
{
    // data members:
    std::string path_entire;
    FILE* file;

public:
    // constructors:
    ~printer();
    printer(const function&, strategy_type, size_t);
    printer() = delete;

    // specific methods:
    printer& operator << (const local_outcome&);
    printer& operator << (std::string&);

    // constant methods:
    const std::string& get_path() const;
};

//------------------------------------------------
// constructors:

printer::~printer()
{
    fclose(file);
}

printer::printer(const function& f, strategy_type imprv, 
    size_t dimension) : path_entire(), file(nullptr)
{
    (path_entire += path_header) += path_folder;

    std::string file_name;
    std::string component_path = "fct_id ";
 
    file_name += component_path;
    path_entire += component_path;

    component_path = std::to_string(f.get_id());
    (file_name += component_path) += " ";
    (path_entire += component_path) += "/";
    
    switch (imprv)
    {
    case strategy_type::best:
        component_path = "improve best";
        break;
    case strategy_type::first:
        component_path = "improve first";
        break;
    case strategy_type::worst:
        component_path = "improve worst";
        break;
    default:
        component_path = "simulated annealing";
        break;
    }

    (file_name += component_path) += " ";
    (path_entire += component_path) += "/";

    component_path = std::to_string(dimension) + "D";
    (file_name += component_path) += path_footer;
    path_entire += file_name;
    
    if (fopen_s(&file, path_entire.c_str(), "w"))
        throw exception_file();

    // header of the file
    fputs(std::string("minimum").c_str(), file);
    fputs(std::string(",\t").c_str(), file);
    fputs(std::string("time_measurement").c_str(), file);
    fputs(std::string("\n").c_str(), file);
}

//------------------------------------------------
// specific methods:

printer& printer::operator << (const local_outcome& o)
{
    fputs(normalize(o.optimum).c_str(), file);
    fputs(",\t", file);
    fputs(std::to_string(o.time_measurement).c_str(), file);
    fputs("\n", file);
    
    return *this;
}

printer& printer::operator << (std::string& s)
{
    fputs(s.c_str(), file);
    return *this;
}

//------------------------------------------------
// constant methods:

const std::string& printer::get_path() const
{
    return path_entire;
}

STD_PSERVICE_END
#endif
