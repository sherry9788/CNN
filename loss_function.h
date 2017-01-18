#ifndef LOSS_FUNCTION
#define LOSS_FUNCTION

#include "base.h"
#include "math_error.h"

double square_error(vector<double> output, vector<double> answer)
{
    if(output.size() != answer.size())
    {
        throw size_error("the size of the output is not fit to answer");
    }

    double ret(0);
    for(int i = 0, Count = output.size(); i < Count; ++i)
    {
        ret += (output[i] - answer[i]) * (output[i] - answer[i]);
    }

    return ret;
}

#endif // LOSS_FUNCTION

