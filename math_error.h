#ifndef MATH_ERROR
#define MATH_ERROR

#include "base.h"

class math_error : public std::logic_error
{
public:

    math_error(string info):
        std::logic_error(info)
    {
        // do nothing
    }

};

#endif // MATH_ERROR

