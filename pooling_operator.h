#ifndef POOLING_OPERATOR_H_INCLUDED
#define POOLING_OPERATOR_H_INCLUDED

#include "base.h"

namespace pooling{

struct op
{
    op();
    virtual double operator()(vector<double> input);
    ~op();
};

double op::operator ()(vector<double>)
{
    throw call_error("trying to call the base version of pooling_op", "pooling_op");
}

op::op()
{
    // do nothing
}

op::~op()
{
    // do nothing
}


template <int area>
struct get_max : public op
{
    get_max():
        op()
    {
        throw call_error("base version of get_max!", "get_max");
    }

    double operator() (vector<double> input)
    {
        // do nothing
        return 0;
    }
};


template <>
struct get_max<4> : public op
{
    double operator()(vector<double> input)
    {
        double curr_max = -1e10;
        for(int i = 0; i < 4; ++i)
        {
            if(input[i] > curr_max)
                curr_max = input[i];
        } // for index i
        return curr_max;
    }
};

template <>
struct get_max<9> : public op
{
    double operator()(vector<double> input)
    {
        double curr_max = -1e10;
        for(int i = 0; i < 9; ++i)
        {
            if(input[i] > curr_max)
                curr_max = input[i];
        } // for index i
        return curr_max;
    }
};


template <int area>
struct average : public op
{
    average()
    {
        throw call_error("base version of average!", "average");
    }

    double operator()(vector<double> input)
    {
        // do nothing
        return 0;
    }
};

template <>
struct average<4> : public op
{
    double operator()(vector<double> input)
    {
        double curr_sum(0);
        for(int i = 0; i < 4; ++i)
        {
            curr_sum += input[i];
        } // for index i
        return curr_sum / 4;
    }
};


template <>
struct average<9> : public op
{
    double operator()(vector<double> input)
    {
        double curr_sum(0);
        for(int i = 0; i < 9; ++i)
        {
            curr_sum += input[i];
        } // for index i
        return curr_sum / 9;
    }
};

} // for pooling namespace

#endif // POOLING_OPERATOR_H_INCLUDED
