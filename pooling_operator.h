#ifndef POOLING_OPERATOR_H_INCLUDED
#define POOLING_OPERATOR_H_INCLUDED

#include "base.h"

namespace pooling{

struct op
{
    virtual double operator()(tensor_t input_tensor);
};


template <int area>
struct get_max : public op
{
    get_max()
    {
        assert(0);
    }

    double operator() (tensor_t input_tensor)
    {
        // do nothing
        return 0;
    }
};


template <>
struct get_max<4> : public op
{
    double operator()(tensor_t input_tensor)
    {
        double curr_max = -1e10;
        for(int i = 0; i < 2; ++i)
        {
            for(int j = 0; j < 2; ++j)
            {
                if(input_tensor[i][j] > curr_max)
                    curr_max = input_tensor[i][j];
            } // for index j
        } // for index i
        return curr_max;
    }
};

template <>
struct get_max<9> : public op
{
    double operator()(tensor_t input_tensor)
    {
        double curr_max = -1e10;
        for(int i = 0; i < 3; ++i)
        {
            for(int j = 0; j < 3; ++j)
            {
                if(input_tensor[i][j] > curr_max)
                    curr_max = input_tensor[i][j];
            } // for index j
        } // for index i
        return curr_max;
    }
};


template <int area>
struct average : public op
{
    average()
    {
        assert(0);
    }

    double operator()(tensor_t input_tensor)
    {
        // do nothing
        return 0;
    }
};

template <>
struct average<4> : public op
{
    double operator()(tensor_t input_tensor)
    {
        double curr_sum(0);
        for(int i = 0; i < 2; ++i)
        {
            for(int j = 0; j < 2 ; ++j)
            {
                curr_sum += input_tensor[i][j];
            } // for index j
        } // for index i
        return curr_sum / 4;
    }
};


template <>
struct average<9> : public op
{
    double operator()(tensor_t input_tensor)
    {
        double curr_sum(0);
        for(int i = 0; i < 3; ++i)
        {
            for(int j = 0; j < 3 ; ++j)
            {
                curr_sum += input_tensor[i][j];
            } // for index j
        } // for index i
        return curr_sum / 9;
    }
};

} // for pooling namespace

#endif // POOLING_OPERATOR_H_INCLUDED
