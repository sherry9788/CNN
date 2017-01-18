#ifndef NONLINEAR_FUNCTION
#define NONLINEAR_FUNCTION

#include "base.h"
#include "math_error.h"

enum nonlinear_function_type
{
    sigmoid,
    ReLU,
    linear
};

class nonlinear_function
{
public:

    nonlinear_function(nonlinear_function_type n_choice = nonlinear_function_type::sigmoid):
        m_choice(n_choice)
    {
        set_function(n_choice);
    }

    double operator()(double input)
    {
        return nonlinear_op(input);
    }

    double derivative(double input)
    {
        return derivative_op(input);
    }

    void set_function(nonlinear_function_type);

private:

    nonlinear_function_type m_choice;
    double (*nonlinear_op)(double);
    double (*derivative_op)(double);
};

namespace nonlinear
{

double sigmoid_op  (double input)
{
    return 1 / (1 + std::exp(-input));
}

double sigmoid_derivative(double input)
{
    double d_tmp = exp(-input);
    return d_tmp / ((1 + d_tmp) + (1 + d_tmp));
}

double linear_op (double input)
{
    return input;
}

double linear_derivative(double)
{
    return 1;
}

double ReLU_op(double input)
{
    if(input > 0)
        return input;
    else
        return 0;
}

double ReLU_derivative(double input)
{
    if(input > 0)
        return 1;
    else
        return 0;
}

} // for namespace nonlinear

void nonlinear_function::set_function(nonlinear_function_type n_choice)
{
    switch(n_choice)
    {
        case nonlinear_function_type::sigmoid :

            nonlinear_op = nonlinear::sigmoid_op;
            derivative_op = nonlinear::sigmoid_derivative;
        break;

        case nonlinear_function_type::ReLU :

            nonlinear_op = nonlinear::ReLU_op;
            derivative_op = nonlinear::ReLU_derivative;
        break;

        default:

            nonlinear_op = nonlinear::linear_op;
            derivative_op = nonlinear::linear_derivative;
        break;
    }
}

#endif // NONLINEAR_FUNCTION

