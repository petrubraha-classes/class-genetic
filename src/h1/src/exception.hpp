#ifndef _0ERROR0_
#define _0ERROR0_

#include <pservice_base>
#include <exception>

STD_PSERVICE_BEGIN

class exception_index : public std::exception
{
public:
	virtual const char* what() const override
	{
		return "error: bad index.\n";
	}
};

class exception_file : public std::exception
{
public:
	virtual const char* what() const override
	{
		return "error: file failed to open.\n";
	}
};

class exception_null : public std::exception
{
public:
	virtual const char* what() const override
	{
		return "error: wrong value.\n";
	}
};

STD_PSERVICE_END
#endif 
