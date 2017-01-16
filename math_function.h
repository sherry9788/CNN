#ifndef MATH_FUNCTION
#define MATH_FUNCTION

namespace math_tool
{

int sqrt(int N)
{
    if(N < 0)
    {
        throw math_error("the argument of sqrt is negative!");
    }

    for(int i = 0; i < N/2; ++i)
    {
        if( i * i == N)
            return i;
    }

    throw math_error("couldn't find a integer solution for sqrt function");

    return N;
}

}

#endif // MATH_FUNCTION

