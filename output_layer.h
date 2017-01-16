#ifndef OUTPUT_LAYER_H_INCLUDED
#define OUTPUT_LAYER_H_INCLUDED

#include "base.h"
#include "layer_common.h"

class output_layer : public layer
{
public:
    output_layer() = delete;
    output_layer(int n_window_size);

    void forward_propagade();
    void backward_propagade();

    std::vector<double> output();

private:

    int m_window_size;
};

void output_layer::forward_propagade()
{
    // do nothing
}

void output_layer::backward_propagade()
{
    // do nothing
}

std::vector<double> output_layer::output()
{
    std::vector<double> ret(m_window_size);
    for(int i = 0; i < m_window_size; ++i)
    {
        ret[i] = m_node[i]->get_activation();
    }
    return ret;
}

output_layer::output_layer(int n_window_size):
    layer(layer_type::able_to_backpropagade),
    m_window_size(n_window_size)
    {
        // do nothing
    }

#endif // OUTPUT_LAYER_H_INCLUDED
