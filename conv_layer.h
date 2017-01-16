#ifndef CONV_LAYER_H_INCLUDED
#define CONV_LAYER_H_INCLUDED

#include "base.h"
#include "kernel.h"

class conv_layer : public layer
{
public:

    void forward_propagade();
    void backward_propagade();

    void add_kernel(kernel n_kernel);

private:

    std::vector<kernel> m_kernel;

};

void conv_layer::add_kernel(kernel n_kernel)
{
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
}

#endif // CONV_LAYER_H_INCLUDED
