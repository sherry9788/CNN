#ifndef POOLING_LAYER_H_INCLUDED
#define POOLING_LAYER_H_INCLUDED

#include "base.h"

#include "layer_common.h"
#include "edge.h"

class pooling_layer : public layer
{
public:

    void backward_propagade();
    void forward_propagade();

private:

    shape2d_t m_area;
    int window_size;

};

void pooling_layer::backward_propagade()
{
    // nothing
    throw call_error("call bp in polling layer", "polling_layer");
}

void pooling_layer::forward_propagade()
{
    for(auto &c : m_edge)
    {
        c->activate();
    }
}

#endif // POOLING_LAYER_H_INCLUDED
