#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <iostream>
      
enum class layer_type
{
    able_to_backpropagate,
    not_able_to_backpropagate
};

template <typename T>
void debug(T x)
{
    std::cout << "---------------"
        << x
        <<       "---------------"
        << std::endl;
}

enum class connecting_type
{
    one_by_one,
    one_to_all
};

#endif // UTIL_H_INCLUDED
