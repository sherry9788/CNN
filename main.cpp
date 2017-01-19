#include "connect.h"
#include "random_function.h"
#include "loss_function.h"

using namespace std;

int main()
{
    input_layer l1(shape2d_t(16, 16));
    conv_layer l2(shape2d_t(3, 3));
    for(int i = 0; i < 3; ++i)
    {
        l2.add_kernel(kernel(3, 3, random_clamp));
    }
    pooling_layer l3(shape2d_t(2, 2));
    conv_layer l4(shape2d_t(3, 3));
    l4.get_connecting_type() = connecting_type::one_by_one;
    for(int i = 0; i < 3; ++i)
    {
        l4.add_kernel(kernel(3, 3, random_clamp));
    }
    pooling_layer l5(shape2d_t(2, 2));
    fully_connected_layer l6;
    fully_connected_layer l7(100);
    output_layer l8(10);

    connect(l1, l2);
    connect(l2, l3);
    connect(l3, l4);
    connect(l4, l5);
    connect(l5, l6);
    connect(l6, l7);
    connect(l7, l8);

    vector<double> input(256, 1);
    vector<double> output(10);

    vector<double> answer(10, 1);

    vector<double> diff(10);


    while(true)
    {
        l1.input_data(input);
        output =  l8.output();
        debug(square_error(answer, output));
        for(int i = 0; i < 10; ++i)
        {
            diff[i] = output[i] - answer[i];
        }
        l8.set_diff(diff);
        l8.backward_propagate();
    }
/*
    input_layer l1(shape2d_t(1, 10));
    fully_connected_layer l2;
    fully_connected_layer l3(100);
    output_layer l4(10);

    connect(l1, l2);
    connect(l2, l3);
    connect(l3, l4);

    vector<double> input(10, 1);
    vector<double> output(10, 1);
    vector<double> answer(10, 1);

    vector<double> diff(10);

    while(true)
    {
        l1.input_data(input);
        output = l4.output();
        debug(square_error(answer, output));
        for(int i = 0; i < 10; ++i)
        {
            diff[i] = output[i] - answer[i];
        }
        l4.set_diff(diff);
        l4.backward_propagate();
    }
    */

    return 0;
}
