#ifndef CONV_LAYER_H_INCLUDED
#define CONV_LAYER_H_INCLUDED

#include "base.h"
#include "kernel.h"
#include "math_function.h"

class conv_layer : public layer
{
public:

    conv_layer() = delete;

    conv_layer(shape2d_t window_size);

    void forward_propagade();
    void backward_propagade();

    void add_kernel(kernel n_kernel);

    shape3d_t get_kernel_size() const;

    vector<kernel> get_kernel() const;

private:

    // Kernel is the scan window and will be used in the initiation
    // of the network
    vector<kernel> m_kernel;
    shape3d_t m_kernel_size;


};

void conv_layer::add_kernel(kernel n_kernel)
{
    if(get<0>(n_kernel.get_area()) != get<1>(m_kernel_size) ||
       get<1>(n_kernel.get_area()) != get<2>(m_kernel_size))
    {
        throw kernel_error("the size is not fit to this layer", n_kernel);
    }
    ++get<0>(m_kernel_size);
    m_kernel.push_back(n_kernel);
}

void conv_layer::forward_propagade()
{
    for(auto &c : m_edge)
    {
        c->activate();
    }
}

void conv_layer::backward_propagade()
{
    // do nothing
    throw call_error("call bp in conv_layer", "conv_layer");
}

conv_layer::conv_layer(shape2d_t window_size):
    layer(layer_type::not_able_to_backpropagade)
{
    get<0>(m_kernel_size) = 0;
    get<1>(m_kernel_size) = get<0>(window_size);
    get<2>(m_kernel_size) = get<1>(window_size);
}

shape3d_t conv_layer::get_kernel_size() const
{
    return m_kernel_size;
}

vector<kernel> conv_layer::get_kernel() const
{
    return m_kernel;
}

#endif // CONV_LAYER_H_INCLUDED
