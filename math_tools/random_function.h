#ifndef RANDOM_FUNCTION
#define RANDOM_FUNCTION

#include "util/util.h"

double random_clamp()
{
    return random() * 2. / RAND_MAX - 1;
}

double all_one()
{
    return 1;
}

vec_double_t random_vector(int size, double (*random_engine)())
{
    vec_double_t dvec(size);
    for(auto &c : dvec)
    {
        c = ptr_double_t(new double(random_engine()));
    }
    return dvec;
}

#endif // RANDOM_FUNCTION

