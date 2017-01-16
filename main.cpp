#include "connect.h"

using namespace std;

int main()
{
    input_layer l1(16);
    l1.get_size() = shape3d_t(1, 4, 4);
    conv_layer l2(shape2d_t(3, 3));

    tensor_t k1 = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
    tensor_t k2 = {{0, 0, 0}, {0, 2, 0}, {0, 0, 0}};

    l2.add_kernel(k1);
    l2.add_kernel(k2);
    output_layer l3(32);

    connect(l1, l2);
    connect(l2, l3);

    return 0;
}
