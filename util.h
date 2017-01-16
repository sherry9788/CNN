#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <iostream>
      
enum class layer_type
{
    able_to_backpropagade,
    not_able_to_backpropagade
};

template <typename T>
void debug(T x)
{
    std::cout << "---------------"
        << x
        <<       "---------------"
        << std::endl;
}

#endif // UTIL_H_INCLUDED
