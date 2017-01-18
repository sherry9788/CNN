#include "connect.h"
#include "random_function.h"
#include "loss_function.h"

using namespace std;

int main()
{
    input_layer l1(shape2d_t(28, 28));
    conv_layer l2(shape2d_t(5, 5));
    for(int i = 0; i < 6; ++i)
    {
        l2.add_kernel(kernel(5, 5, random_clamp));
    }
    pooling_layer l3(shape2d_t(2, 2));
    conv_layer l4(shape2d_t(5, 5));
    for(int i = 0; i < 6; ++i)
    {
        l4.add_kernel(kernel(5, 5, random_clamp));
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

    vector<double> input(784, 1);
    l1.input_data(input);
    vector<double> output = l8.output();

    return 0;
}
