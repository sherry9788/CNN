#include "connect.h"

using namespace std;

int main()
{
    input_layer l1(10);
    output_layer l2(10);
    connect(l1, l2);

    l1.input_data({1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
    l1.forward_propagade();

    auto output = l2.output();

    for(auto c : output)
        cout << c << endl;

    return 0;
}
