#ifndef MATH_ERROR
#define MATH_ERROR

#include "util/dependence.h"

class math_error : public std::logic_error
{
public:

    math_error(std::string info):
        std::logic_error(info)
    {
        // do nothing
    }

};

#endif // MATH_ERROR

